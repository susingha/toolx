/*
 * This was given by Adam from CA to test the json functioning.
 * Keeping this inside the test folder
 *
 */

#include "../common/policy_helper.h"
#include "../shmem_setter/policy_store.h"
#include <errno.h>
#include <mhash.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void browseAll();
void displayAdmin();
void displayControl();
void displaySession();
void browseResourcePolicies();
void browseObligationPolicies();
void browseCalendars();
void browseTimeBlocks(int cal_num);
void browseIncludeTimeBlocks(int cal_num);    
void browseExcludeTimeBlocks(int cal_num);
void browseLocations();
void browseGeoFences();
void editAll();
void checkLogon();
void playground();

unsigned long *shared;

int main(){
  char input[255], in;
  
  int shmfd = shm_open(SHM_PATH, O_RDWR, 0660);
    
  if(shmfd <= 0){
    int err = errno;
    printf("Could not open shared memory. FD: %d.\nERRNO: %s\nReturning 1.\n", shmfd, strerror(err));
    return 1;
  } 
  
  int shmsize = getInitSHMSize();
  
  if(shmsize < 0) {
  printf("Shared memory could not be open. Does it exist?\n");
  return 3;
  }

  shared = mmap(NULL, shmsize, PROT_WRITE | PROT_READ, MAP_SHARED, shmfd, 0);  

  if(shared == MAP_FAILED){
    printf("Failed to map memory.... quitting.\n");
  return 2;
  }

  printf("\n\nSuccessfully loaded %d bytes at %p\n\n", shmsize, shared);
  
  while(1){
    printf("----------------------------------\n");
    printf("Policy Browser:\n");
    printf("----------------------------------\n");
    printf("e] Edit Shared Memory Fields\n");
    printf("v] View Shared Memory Fields\n");
    printf("@] Adam's Testing Playground\n");
    printf("q] Quit\n");
    checkLogon();
    printf("\n");
    printf("Enter A Selection: ");


    fgets(input, 255, stdin);
    in = input[0];
    printf("\n");
    
    if(in == 'e'){
      editAll();
    } else if(in == 'v'){
      browseAll();
    } else if(in == '@'){
      playground();
    } else if(in == 'q'){
    exit(0);
    } else printf("Invalid Selection [%s]!\n", input);
    printf("\n");
  }
  
  return 0;
}


void playground(){  
  char *device_name = "Adam's Phone";
  char *devicemodel = "iPhone 4";
  char *deviceos = "iOS 6";
  char *deviceosversion = "6.3.4.21.3.14.3";
  char *carrier = "Verizon Wireless";
  char *timezone = "New York";
  char *appleid = "przad01";
  char *email = "przad01@contractor.ca.com";
  char *language = "US English";
  char *ssid = "CAatCEWIT";
  char *vpn = "CA Tech";

  double longitude = 40.9;
  double latitude = -73.14;

  int passcodeLockTime = 3600;
  int isConnectedLAN = 1;
  int isConnectedWAN = 1;
  int isClipboardEnabled = 1;
  int isLocationEnabled = 1;
  int isCameraEnabled = 1;
  int isSpeakerEnabled = 1;
  int isCellModemEnabled = 1;
  int isMicEnabled = 1;
  int isTelephonyEnabled = 1;
  int isJailbroken = 1;
  int isRoaming = 1;
  int isHTTPProxyActive = 1;
  int isBluetoothActive = 1;
  int isProvisioningProfilePresent = 1;
  int isAirplaneModeActive = 1;
  int isiCloudActive = 1;
  int isPasscodeEnabled = 1;
  int matchingPolicyAction = POLICY_ALLOW;
  int seclevel = MODE_FAIL;
  int AuditAdmin = AUDIT_OFF;
  int AuditSecurityViolation = AUDIT_ON;
  int AuditSecurityCheck = AUDIT_OFF;
  int AuditLogin = AUDIT_ON;


  setDeviceName(shared, device_name);
  setDeviceModel(shared, devicemodel);
  setDeviceOS(shared, deviceos);
  setDeviceOSVersion(shared, deviceosversion);
  setCarrier(shared, carrier);
  setTimeZone(shared, timezone);
  setAppleID(shared, appleid);
  setEMailAccount(shared, email);
  setLanguage(shared, language);
  setNetworkSSID(shared, ssid);
  setVPNHost(shared, vpn);
  setCurrentLocationLongitude(shared, longitude);
  setCurrentLocationLatitude(shared, latitude);
  setPasscodeLockTime(shared, passcodeLockTime);
  setConnectedNetwork(shared, isConnectedLAN);
  setConnectedInternet(shared, isConnectedWAN);
  setClipboardEnabled(shared, isClipboardEnabled);
  setLocationEnabled(shared, isLocationEnabled);
  setCameraEnabled(shared, isCameraEnabled);
  setSpeakerEnabled(shared, isSpeakerEnabled);
  setCellModemEnabled(shared, isCellModemEnabled);
  setMicrophoneEnabled(shared, isMicEnabled);
  setTelephonyEnabled(shared, isTelephonyEnabled);
  setJailbroken(shared, isJailbroken);
  setRoaming(shared, isRoaming);
  setHTTPProxyActive(shared, isHTTPProxyActive);
  setBluetoothEnabled(shared, isBluetoothActive);
  setProvisioningProfilePresent(shared, isProvisioningProfilePresent);
  setAirplaneMode(shared, isAirplaneModeActive);
  setiCloudActive(shared, isiCloudActive);
  setPasscodeEnabled(shared, isPasscodeEnabled);
  setNoMatchingPolicyAction(shared, matchingPolicyAction);
  setSecurityLevel(shared, seclevel);
  setAuditAdmin(shared, AuditAdmin);
  setAuditChecks(shared, AuditSecurityCheck);
  setAuditSecurityViolation(shared, AuditSecurityViolation);
  setAuditLogin(shared, AuditLogin);

  while(1){
    char input[256];

    printf("Enter A Obligation Policy Name: ");

    fgets(input, 255, stdin);

    size_t ln = strlen(input) - 1;
    if (input[ln] == '\n')
      input[ln] = '\0';


    printf("\n");
    printf("\n");
    printf("Checking [%s]\n", input);
    int rv = searchObligationPolicies(shared, input);
    printf("Result: %d\n", rv);

    if(input[0] == '!'){
      return;
    }
  } 
}


void checkLogon(){
  if(getSessionLogonStatus(shared) != LOGGED_IN){
    printf("----------------------------------\n");
    printf("WARNING: USER IS NOT CURRENTLY LOGGED ON!\n");
    printf("----------------------------------\n");
  }
}


void editAll(){
  char input[255], in;
  int status;

  while(1){
    status = getSessionLogonStatus(shared);

    printf("----------------------------------\n");
    printf("Shared Memory Editor:\n");
    printf("----------------------------------\n");
    printf("l] Toggle Logged On Status [Current: %d]\n", status);
    printf("c] Clear Shared Memory\n");
    printf("m] Return to Main Menu\n");
    checkLogon();
    printf("\n");
    printf("Enter A Selection: ");
   

    fgets(input, 255, stdin);
    in = input[0];
    printf("\n");
    
    if(in == 'l'){
      if(status == LOGGED_IN){
        setSessionLogonStatus(shared, LOGGED_OUT);
      } else if(status == LOGGED_OUT){
        setSessionLogonStatus(shared, LOGGED_IN);
      } else printf("Failed to toggle status!\n");
    
      printf("Logged On Status is now %d\n", getSessionLogonStatus(shared));
    } else if(in == 'c'){
    clear_shmem(shared);
    printf("Successfully Cleared Shared Memory\n");
    printf("\n");
    exit(0);

    } else if(in == 'm'){
    return;

    } else printf("Invalid Selection [%s]!\n", input);
    printf("\n");
  }
}


void browseAll(){
  char input[255], in;
  
  while(1){
    printf("----------------------------------\n");
    printf("Policy Browser:\n");
    printf("----------------------------------\n");
    printf("a] Administration Information\n");
    printf("d] Device Control Block\n");
    printf("s] Session Information\n");
    printf("u] Resource Policies\n");
    printf("o] Obligation Policies\n");
    printf("c] Calendars\n");
    printf("g] GeoFences\n");
    printf("l] Locations\n");
    printf("m] Return to Main Menu\n");
    checkLogon();
    printf("\n");
    printf("Enter A Selection: ");
   

    fgets(input, 255, stdin);
    in = input[0];
    printf("\n");
    
    if(in == 'a'){
      displayAdmin();
    } else if(in == 'd'){
      displayControl();
    } else if(in == 's'){
      displaySession();
    } else if(in == 'u'){
      browseResourcePolicies();
    } else if(in == 'o'){
      browseObligationPolicies();
    } else if(in == 'c'){
      browseCalendars();
    } else if(in == 'g'){
      browseGeoFences();
    } else if(in == 'l'){
      browseLocations();
    } else if(in == 'm'){
      return;
    } else {
      printf("Invalid Selection [%s]!\n", input);      
    } 

    printf("\n");
  }
}



void displayAdmin(){
  int shmsize = getAdminSHMSize(shared);
  
  printf("----------------------------------\n");
  printf("Admin Block Information:\n");
  printf("----------------------------------\n");
  printf("Shared Memory Size: %d bytes\n", shmsize);  
}


void displayControl(){
  char *devicename = getDeviceName(shared);
  char *devicemodel = getDeviceModel(shared);
  char *deviceos = getDeviceOS(shared);
  char *deviceosversion = getDeviceOSVersion(shared);
  char *carrier = getCarrier(shared);
  char *timezone = getTimeZone(shared);
  char *appleid = getAppleID(shared);
  char *email = getEMailAccount(shared);
  char *language = getLanguage(shared);
  char *ssid = getNetworkSSID(shared);
  char *vpn = getVPNHost(shared);
  double longitude = getCurrentLocationLongitude(shared);
  double latitude = getCurrentLocationLatitude(shared);
  int passcodeLockTime = getPasscodeLockTime(shared);
  int isClipboardEnabled = isEnabled_Clipboard(shared);
  int isLocationEnabled =isEnabled_Location(shared);
  int isCameraEnabled = isEnabled_Camera(shared);
  int isSpeakerEnabled = isEnabled_Speaker(shared);
  int isCellModemEnabled = isEnabled_CellModem(shared);
  int isMicEnabled = isEnabled_Microphone(shared);
  int isTelephonyEnabled = isEnabled_Telephony(shared);
  int isBluetoothActive = isEnabled_Bluetooth(shared);
  int isPasscodeEnabled = isEnabled_Passcode(shared);
  int isConnectedLAN = isActive_Network(shared);
  int isConnectedWAN = isActive_Internet(shared);
  int isJailbroken = isActive_Jailbroken(shared);
  int isRoaming = isActive_Roaming(shared);
  int isHTTPProxyActive = isActive_HTTPProxy(shared);
  int isProvisioningProfilePresent = isActive_ProvisioningProfile(shared);
  int isAirplaneModeActive = isActive_AirplaneMode(shared);
  int isiCloudActive = isActive_iCloud(shared);
  int matchingPolicyAction = getNoMatchingPolicyAction(shared);
  int seclevel = getSecurityLevel(shared);
  int AuditAdmin = isAuditAdmin(shared);
  int AuditSecurityViolation = isAuditSecurityViolation(shared);
  int AuditSecurityCheck = isAuditChecks(shared);
  int AuditLogin = isAuditLogin(shared);

  printf("----------------------------------\n");
  printf("Device Control Block Contents:\n");
  printf("----------------------------------\n");
  printf("Device Name: %s\n", devicename);  
  printf("Device Model: %s\n", devicemodel);  
  printf("Device OS: %s\n", deviceos);  
  printf("Device OS Version: %s\n", deviceosversion);  
  printf("Carrier Name: %s\n", carrier);  
  printf("Time Zone: %s\n", timezone);  
  printf("Apple ID: %s\n", appleid);  
  printf("E-Mail Address: %s\n", email);  
  printf("Language: %s\n", language);  
  printf("Network SSID: %s\n", ssid);  
  printf("VPN Host: %s\n", vpn);  
  printf("Current Latitude: %e\n", latitude);  
  printf("Current Longitude: %e\n", longitude);  
  printf("Passcode Lock Time (s): %d\n", passcodeLockTime);
  printf("Passcode Enabled: %d\n", isPasscodeEnabled);
  printf("Clipboard Enabled: %d\n", isClipboardEnabled);
  printf("Location Enabled: %d\n", isLocationEnabled);
  printf("Camera Enabled: %d\n", isCameraEnabled);
  printf("Speaker Enabled: %d\n", isSpeakerEnabled);
  printf("Cell Modem Enabled: %d\n", isCellModemEnabled);
  printf("Microphone Enabled: %d\n", isMicEnabled);
  printf("Telephony Enabled: %d\n", isTelephonyEnabled);
  printf("JailBroken Status: %d\n", isJailbroken);
  printf("LAN Connection Status: %d\n", isConnectedLAN);
  printf("WAN Connection Status: %d\n", isConnectedWAN);
  printf("Roaming Status: %d\n", isRoaming);
  printf("HTTP Proxy Status: %d\n", isHTTPProxyActive);
  printf("Bluetooth Status: %d\n", isBluetoothActive);
  printf("Provisioning Profile Status: %d\n", isProvisioningProfilePresent);
  printf("Airplane Mode Status: %d\n", isAirplaneModeActive);
  printf("isiCloudActive: %d\n", isiCloudActive);
  printf("No Matching Policy Action: %d\n", matchingPolicyAction);
  printf("Security Level: %d\n", seclevel);
  printf("[Audit] Admin: %d\n", AuditAdmin);
  printf("[Audit] Security Violation: %d\n", AuditSecurityViolation);
  printf("[Audit] Security Check: %d\n", AuditSecurityCheck);
  printf("[Audit] Login: %d\n", AuditLogin);
}


void displaySession(){
  int logged_on = getSessionLogonStatus(shared);
  int admin = isAdmin(shared);
  int tenant_id = getSessionTenantID(shared);
  long timestamp = getSessionTimeStamp(shared);
  char *id = getSessionID(shared);
  char *name = getSessionUserID(shared);  
  char *password = getSessionHashedPassword(shared);  
  
  printf("----------------------------------\n");
  printf("Session Information:\n");
  printf("----------------------------------\n");
  printf("Session TimeStamp: %ld\n", timestamp);  
  printf("Session Administrator Status: %d\n", admin);  
  printf("Session Logged On Status: %d\n", logged_on);  
  printf("Session Tenant ID: %d\n", tenant_id);  
  printf("Session ID: %s\n", id);  
  printf("Session User ID: %s\n", name);  
  printf("Session Password Hash: %s\n", password);  
}


void browseResourcePolicies(){
  int policy_count, index;
  char *name, *description;
  char *calendar, *location, *obligations;
  char *resource_class, *resources, *actions;
  char *users, *groups, *filters;
  int explicit_deny;
  char input[255];

   // Let's play in the data:
  policy_count = getNumberOfResourcePolicies(shared);
  
  if(policy_count < 1){
    printf("No Resource Policies Found!\n");
    return;
  }
    
  while(1){
    printf("----------------------------------\n");
    printf("Resource Policy Menu:\n");
    printf("----------------------------------\n");
    printf("1-%d] View Policy #\n", policy_count);
    printf("m] Back to Main Menu\n");
    checkLogon();
    printf("\n");
    printf("Enter A Selection: ");

    fgets(input, 255, stdin);
    printf("\n");
  
    if(!strcmp(input, "m\n")) {
      return;
    } else {
    
      index = atoi(input);
      if(0 >= index || index > policy_count) {
        printf("That Policy doesn't exist!\n");
      } else {
        index--;
        name = getResourcePolicyName(shared, index);
        description = getResourcePolicyDescription(shared, index);
        explicit_deny = getResourcePolicyExplicitDeny(shared, index);
        users = getResourcePolicyUsers(shared, index);
        groups = getResourcePolicyGroups(shared, index);
        calendar = getResourcePolicyCalendar(shared, index);
        location = getResourcePolicyLocation(shared, index);
        filters = getResourcePolicyFilters(shared, index);
        obligations = getResourcePolicyPossibleObligations(shared, index);
        resource_class = getResourcePolicyResourceClass(shared, index);
        resources = getResourcePolicyResources(shared, index);
        actions = getResourcePolicyActions(shared, index);

        printf("----------------------------------\n");
        printf("Policy #%d:\n", index + 1);      
        printf("----------------------------------\n");
        printf("Name: %s\n", name);  
        printf("Description: %s\n", description);  
        printf("Explicit Deny: %d\n", explicit_deny);      
        printf("Users: %s\n", users);      
        printf("Groups: %s\n", groups);      
        printf("Calendar: %s\n", calendar);      
        printf("Location: %s\n", location);  
        printf("Possible Obligation Policies: %s\n", obligations);      
        printf("Filters: %s\n", filters);      
        printf("Resource Class: %s\n", resource_class);      
        printf("Resources: %s\n", resources);  
        printf("Actions: %s\n", actions);      
      }
    }
    printf("\n");
  }
}     




void browseCalendars(){
  int calendar_count, index;
  char *name, *description;
  char input[255];
  unsigned long calStartDate, calEndDate;
  int calNumIncludeBlocks, calNumExcludeBlocks;

   // Let's play in the data:
  calendar_count = getNumberOfCalendars(shared);
  
  if(calendar_count < 1){
    printf("No Calendars Found!\n");
    return;
  }
    
  while(1){
    printf("----------------------------------\n");
    printf("Calendar Menu:\n");
    printf("----------------------------------\n");
    printf("1-%d] Load Calendar #\n", calendar_count);
    printf("m] Back to Main Menu\n");
  checkLogon();
    printf("\n");
    printf("Enter A Selection: ");

    fgets(input, 255, stdin);
    printf("\n");
  
    if(!strcmp(input, "m\n")) {
      return;
    } else {
    
      index = atoi(input);
      if(0 >= index || index > calendar_count) {
        printf("That Calendar doesn't exist!\n");
      } else {
        index--;
        name = getCalendarName(shared, index);
        description = getCalendarDescription(shared, index);
        calStartDate = getCalendarStartDate(shared, index);
        calEndDate = getCalendarEndDate(shared, index);
        calNumIncludeBlocks = getCalendarNumIncludeBlocks(shared, index);
        calNumExcludeBlocks = getCalendarNumExcludeBlocks(shared, index);

        printf("----------------------------------\n");
      printf("Calendar #%d:\n", index + 1);      
        printf("----------------------------------\n");
        printf("Name: %s\n", name);  
        printf("Description: %s\n", description);  
        printf("Start Date: %lu\n", calStartDate);  
        printf("End Date: %lu\n", calEndDate);  
        printf("Number of Include Time Blocks: %d\n", calNumIncludeBlocks);
        printf("Number of Exclude Time Blocks: %d\n\n", calNumExcludeBlocks);
        
        browseTimeBlocks(index);
    }
    }
    printf("\n");
  }
}     


void browseTimeBlocks(int cal_num){
  char input[255];
  int inc_tb_count = getCalendarNumIncludeBlocks(shared, cal_num);
  int exc_tb_count = getCalendarNumExcludeBlocks(shared, cal_num);
      
  while(1){
    printf("----------------------------------\n");
    printf("Calendar #%d Menu:\n", cal_num + 1);
    printf("----------------------------------\n");
    printf("i] Load Include Time Block Menu\n");
    printf("e] Load Exclude Time Block Menu\n");
    printf("m] Return to Calendar Menu\n");
  checkLogon();
    printf("\n");
    printf("Enter A Selection: ");

    fgets(input, 255, stdin);
    printf("\n");

    if(!strcmp(input, "m\n")) {
      return;
    } else if(!strcmp(input, "i\n")) {
    if(inc_tb_count)
        browseIncludeTimeBlocks(cal_num);   
      else
          printf("Sorry, there are no include time blocks for this calendar!\n");
        
    } else if(!strcmp(input, "e\n")) {

      if(exc_tb_count)
        browseExcludeTimeBlocks(cal_num);   
      else
          printf("Sorry, there are no exclude time blocks for this calendar!\n");

    } else {
      printf("Unknown Command\n");
    }
  }
}


void browseIncludeTimeBlocks(int cal_num){
  int index, tb_count;
  int tb_id, tb_weekdays, tb_monthdays;
  int tb_months, tb_startTime;
  int tb_duration, tb_repeatInterval;
  char input[255];

  tb_count = getCalendarNumIncludeBlocks(shared, cal_num);
  
  while(1){
    printf("----------------------------------\n");
    printf("Calendar #%d Include Time Block Menu:\n", cal_num + 1);
    printf("----------------------------------\n");
    printf("1-%d] Load Include Time Block #\n", tb_count);
    printf("m] Back to Main Menu\n");
    checkLogon();
    printf("\n");
    printf("Enter A Selection: ");

    fgets(input, 255, stdin);
    printf("\n");

    if(!strcmp(input, "m\n")) {
      return;
    } else {
    
      index = atoi(input);
      if(0 >= index || index > tb_count) {
        printf("That Time Block doesn't exist!\n");
      } else {
        index--;
        
        tb_id = getIncludeTimeBlockID(shared, cal_num, index);
        tb_weekdays = getIncludeTimeBlockWeekdays(shared, cal_num, index);
        tb_monthdays = getIncludeTimeBlockMonthdays(shared, cal_num, index);
        tb_months = getIncludeTimeBlockMonths(shared, cal_num, index);
        tb_startTime = getIncludeTimeBlockStartTime(shared, cal_num, index);
        tb_duration = getIncludeTimeBlockDuration(shared, cal_num, index);
        tb_repeatInterval = getIncludeTimeBlockRepeatInterval(shared, cal_num, index);
        
        printf("----------------------------------\n");
      printf("Calendar #%d Include Block %d:\n", cal_num, index + 1);      
        printf("----------------------------------\n");
        printf("ID: %d\n", tb_id);  
        printf("Weekdays: %d\n", tb_weekdays);  
        printf("Monthdays: %d\n", tb_monthdays);  
        printf("Months: %d\n", tb_months);  
        printf("Start Time: %d\n", tb_startTime);
        printf("Duration: %d\n", tb_duration);
        printf("Repeat Interval: %d\n\n", tb_repeatInterval);
    }
    }
  }
}


void browseExcludeTimeBlocks(int cal_num){
  int index, tb_count;
  int tb_id, tb_weekdays, tb_monthdays;
  int tb_months, tb_startTime;
  int tb_duration, tb_repeatInterval;
  char input[255];

  tb_count = getCalendarNumExcludeBlocks(shared, cal_num);
  
  while(1){
    printf("----------------------------------\n");
    printf("Calendar #%d Exclude Time Block Menu:\n", cal_num + 1);
    printf("----------------------------------\n");
    printf("1-%d] Load Exclude Time Block #\n", tb_count);
    printf("m] Back to Main Menu\n");
  checkLogon();
    printf("\n");
    printf("Enter A Selection: ");

    fgets(input, 255, stdin);
    printf("\n");

    if(!strcmp(input, "m\n")) {
      return;
    } else {
    
      index = atoi(input);
      if(0 >= index || index > tb_count) {
        printf("That Time Block doesn't exist!\n");
      } else {
        index--;
        
        tb_id = getExcludeTimeBlockID(shared, cal_num, index);
        tb_weekdays = getExcludeTimeBlockWeekdays(shared, cal_num, index);
        tb_monthdays = getExcludeTimeBlockMonthdays(shared, cal_num, index);
        tb_months = getExcludeTimeBlockMonths(shared, cal_num, index);
        tb_startTime = getExcludeTimeBlockStartTime(shared, cal_num, index);
        tb_duration = getExcludeTimeBlockDuration(shared, cal_num, index);
        tb_repeatInterval = getExcludeTimeBlockRepeatInterval(shared, cal_num, index);
        
        printf("----------------------------------\n");
      printf("Calendar #%d Exclude Block %d:\n", cal_num, index + 1);      
        printf("----------------------------------\n");
        printf("ID: %d\n", tb_id + 1);  
        printf("Weekdays: %d\n", tb_weekdays);  
        printf("Monthdays: %d\n", tb_monthdays);  
        printf("Months: %d\n", tb_months);  
        printf("Start Time: %d\n", tb_startTime);
        printf("Duration: %d\n", tb_duration);
        printf("Repeat Interval: %d\n\n", tb_repeatInterval);
    }
    }
  }
}


void browseGeoFences(){
  int geofence_count, index, i;
  char input[255];
  char *name, *description;
  int id, numCoords;

   // Let's play in the data:
  geofence_count = getNumberOfGeoFences(shared);
  
  if(geofence_count < 1){
    printf("No GeoFences Found!\n");
    return;
  }
    
  while(1){
    printf("----------------------------------\n");
    printf("GeoFence Menu:\n");
    printf("----------------------------------\n");
    printf("1-%d] Load GeoFence #\n", geofence_count);
    printf("m] Back to Main Menu\n");
    checkLogon();
    printf("\n");
    printf("Enter A Selection: ");

    fgets(input, 255, stdin);
    printf("\n");
  
    if(!strcmp(input, "m\n")) {
      return;
    } else {
    
      index = atoi(input);
      if(0 >= index || index > geofence_count) {
        printf("That GeoFence doesn't exist!\n");
      } else {
        index--;
        id = getGeoFenceID(shared, index);
        name = getGeoFenceName(shared, index);
        description = getGeoFenceDescription(shared, index);
        numCoords = getGeoFenceNumCoordinates(shared, index);

        printf("----------------------------------\n");
        printf("GeoFence #%d:\n", index + 1);      
        printf("----------------------------------\n");
        printf("ID: %d\n", id);
        printf("Name: %s\n", name);  
        printf("Description: %s\n", description);  
        printf("# of Coordinates: %d\n", numCoords);

        for(i=0; i<numCoords; i++){
          Coordinate *coord = getGeoFenceCoordinate(shared, index, i);
          printf("\t Coordinate %d: [X: %e] [Y: %e]\n", i+1, coord->latitude, coord->longitude);
        }
      }
    } printf("\n");
  }
}     


void browseLocations(){
  int location_count, index, i;
  char input[255];
  char *name, *description;
  int id, numGeofences;
  int numCoords;
  int j;
  GeoFence *gf;

   // Let's play in the data:
  location_count = getNumberOfLocations(shared);
  
  if(location_count < 1){
    printf("No Locations Found!\n");
    return;
  }
    
  while(1){
    printf("----------------------------------\n");
    printf("Location Menu:\n");
    printf("----------------------------------\n");
    printf("1-%d] Load Location #\n", location_count);
    printf("m] Back to Main Menu\n");
    checkLogon();
    printf("\n");
    printf("Enter A Selection: ");

    fgets(input, 255, stdin);
    printf("\n");
  
    if(!strcmp(input, "m\n")) {
      return;
    } else {
    
      index = atoi(input);
      if(0 >= index || index > location_count) {
        printf("That Location doesn't exist!\n");
      } else {
        index--;

        printf("----------------------------------\n");
        printf("Location #%d:\n", index + 1);      
        printf("----------------------------------\n");
        id = getLocationID(shared, index);
        printf("ID: %d\n", id);
        name = getLocationName(shared, index);
        printf("Name: %s\n", name);  
        description = getLocationDescription(shared, index);
        printf("Description: %s\n", description);  
        numGeofences = getLocationNumGeoFences(shared, index);
        printf("# of Coordinates: %d\n", numGeofences);

        for(i=0; i<numGeofences; i++){
          gf = getLocationGeoFence(shared, index, i);
          printf("\n");
          printf("\t GeoFence %d ID: %d\n", i+1, getGeoFenceIDByStruct(shared, gf));
          printf("\t GeoFence %d Name: %s\n", i+1, getGeoFenceNameByStruct(shared, gf));
          printf("\t GeoFence %d Description: %s\n", i+1, getGeoFenceDescriptionByStruct(shared, gf));

          numCoords = getGeoFenceNumCoordinatesByStruct(shared, gf);
          for(j=0; j<numCoords; j++){
            Coordinate *coord = getGeoFenceCoordinateByStruct(shared, gf, j);
            printf("\t GeoFence Coordinate %d: [X: %e] [Y: %e]\n", j+1, coord->latitude, coord->longitude);
          }
        }
      }
    } printf("\n");
  }
}


void browseObligationPolicies(){
  int obpolicy_count, index;
  char *name, *description, *filters;
  char *calendar, *location, *obligations;
  char *resource_class, *resources, *actions;
  char input[255];

   // Let's play in the data:
  obpolicy_count = getNumberOfObligationPolicies(shared);
  
  if(obpolicy_count < 1){
    printf("No Obligation Policies Found!\n");
    return;
  }
    
  while(1){
    printf("----------------------------------\n");
    printf("Obligation Policy Menu:\n");
    printf("----------------------------------\n");
    printf("1-%d] View Obligation Policy #\n", obpolicy_count);
    printf("m] Back to Main Menu\n");
    checkLogon();
    printf("\n");
    printf("Enter A Selection: ");

    fgets(input, 255, stdin);
    printf("\n");
  
    if(!strcmp(input, "m\n")) {
      return;
    } else {
    
      index = atoi(input);
      if(0 >= index || index > obpolicy_count) {
        printf("That Obligation Policy doesn't exist!\n");
      } else {
        index--;
        name = getObligationPolicyName(shared, index);
        description = getObligationPolicyDescription(shared, index);
        calendar = getObligationPolicyCalendar(shared, index);
        location = getObligationPolicyLocation(shared, index);
        obligations = getObligationPolicyObligations(shared, index);
        filters = getObligationPolicyFilters(shared, index);
        resource_class = getObligationPolicyResourceClass(shared, index);
        resources = getObligationPolicyResources(shared, index);
        actions = getObligationPolicyResourceActions(shared, index);

        printf("----------------------------------\n");
        printf("Obligation Policy #%d:\n", index + 1);      
        printf("----------------------------------\n");
        printf("Name: %s\n", name);  
        printf("Description: %s\n", description);  
        printf("Calendar: %s\n", calendar);      
        printf("Location: %s\n", location);  
        printf("Obligations: %s\n", obligations);      
        printf("Filters: %s\n", filters);      
        printf("Resource Class: %s\n", resource_class);      
        printf("Resources: %s\n", resources);  
        printf("Actions: %s\n", actions);      
      }
    }
    printf("\n");
  }
}  



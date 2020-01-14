#!/usr/bin/python

from selenium import webdriver
import pickle
import time
import sys
import datetime

### script hosted at: https://github.com/susingha/toolx/blob/master/scripts/autochrome.py


### used on dev pc ###
chromebin = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe"
wedriverurl = "C:/Users/Marlinspike/bin/chromedriver.exe"
norecsruntime = 50
waitdef = 3  # seconds

### used on deployment server windows ###
chromebin = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe"
wedriverurl = "C:/Users/supratik/bin/chromedriver.exe"
norecsruntime = 1000
waitdef = 60 # seconds

### used on deployment server ubuntu ###
chromebin = "/usr/bin/google-chrome-stable"
wedriverurl = "/home/susingha/bin/chromedriver"
norecsruntime = 1000
waitdef = 60 # seconds

print (sys.version)
options = webdriver.ChromeOptions()
options.add_argument('--ignore-certificate-errors')
options.add_argument("--test-type")
options.binary_location = chromebin
driver = webdriver.Chrome(wedriverurl)
driver.maximize_window()

driver.get('https://www.linkedin.com')

open("cookiesforlinkedin.pkl", "ab").close()

print "Loading cookies"
try:
    for cookie in pickle.load(open("cookiesforlinkedin.pkl", "rb")):
        driver.add_cookie(cookie)
except:
    print "No Cookies found"

    
driver.refresh()
print "Make sure browser is logged into desired account. If not, login manually using username and password"
x = raw_input("Press Enter to save cookies. x to exit: ")
if x == 'x' or x == 'X' or x == 'q' or x == 'Q':
    exit()

print "Saving cookies"
pickle.dump(driver.get_cookies() , open("cookiesforlinkedin.pkl","wb"))

driver.get('https://www.linkedin.com/feed/following/')
print "Finding people you follow ..."
unfollowbuttons = driver.find_elements_by_xpath("//button[@data-control-name='actor_follow_toggle' and @aria-pressed='true' and @aria-label='Follow' and @class='follows-recommendation-card__follow-btn artdeco-button artdeco-button--tertiary artdeco-button--1 follow is-following ember-view']")
totaltounfollow = len(unfollowbuttons)

print "found", totaltounfollow, "people to unfollow"

count = 0
try_later = []
for i in xrange(totaltounfollow):
    if unfollowbuttons[i].is_enabled() and unfollowbuttons[i].is_displayed():
	unfollowbuttons[i].click()
	count += 1
	print "Unfollowed", count, "person"
    else:
	print "Button", count, "is not clickable"
	try_later.append(unfollowbuttons[i])

    time.sleep(2)




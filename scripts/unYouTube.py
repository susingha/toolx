#!/usr/bin/python

from selenium import webdriver
from selenium.webdriver.common.action_chains import ActionChains
from selenium.common.exceptions import NoSuchElementException

import pickle
import time
import sys
import datetime

# script hosted at: https://github.com/susingha/toolx/blob/master/scripts/autochrome.py

DEMO_MODE = False

# used on Cisco deployment server windows #
chromebin = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe"
webdriverurl = "C:/Users/supratik/bin/chromedriver.exe"
nokillsruntime = 1000
waitdef = 60 # seconds

# used on dev pc #
chromebin = "C:/Program Files (x86)/Google/Chrome/Application/chrome.exe"
webdriverurl = "C:/Users/Marlinspike/bin/chromedriver.exe"
nokillsruntime = 1000 # 50
waitdef = 60 # 3  # seconds

# used on garage server ubuntu #
chromebin = "/usr/bin/google-chrome-stable"
webdriverurl = "/home/cisco/bin/chromedriver"
nokillsruntime = 1000
waitdef = 60 # seconds

# used on deployment server ubuntu #
chromebin = "/usr/bin/google-chrome-stable"
webdriverurl = "/home/susingha/bin/chromedriver"
nokillsruntime = 50
waitdef = 3 # seconds

# used on deployment server ubuntu vmware # 
chromebin = "/usr/bin/google-chrome-stable"
webdriverurl = "/home/ssingha/bin/chromedriver"
nokillsruntime = 50
waitdef = 3 # seconds



print (sys.version)
options = webdriver.ChromeOptions()
options.add_argument("user-data-dir=selenium")
#ptions.add_argument("--headless") # not needed
options.add_argument('--ignore-certificate-errors')
options.add_argument("--test-type")
options.binary_location = chromebin
driver = webdriver.Chrome(webdriverurl, chrome_options=options)
driver.get('https://www.youtube.com')

print "Loading cookies"
COOKIE_FILE = "cookies.nochekin.pkl"
open(COOKIE_FILE, "ab").close()

"""
cookiefd = open(COOKIE_FILE, "rb")
cookies = pickle.load(cookiefd)
for cookie in cookies:
    driver.add_cookie(cookie)
cookiefd.close()
print "Loaded ", len(cookies), "cookies"
"""

driver.refresh()
print "Make sure browser is logged into desired account. If not, login manually using username and password"
x = raw_input("Press Enter to save cookies. x to exit: ")
if x == 'x' or x == 'X' or x == 'q' or x == 'Q':
    exit()

print "Saving cookies"
cookiefd = open(COOKIE_FILE, "wb")
pickle.dump(driver.get_cookies(), cookiefd)
cookiefd.close()

# driver.maximize_window()

totaldest = 0
totalrecs = 0
nokills = 0
wait = waitdef

scrollpause = 3
scrollerjs = "document.body.scrollHeight || document.documentElement.scrollHeight"

try:
    while True:
        print "Current date and time:"
        print str(datetime.datetime.now())
        print
        print "About to remove more recommendations in", wait, "seconds"
        driver.refresh()
        time.sleep(wait)


        if DEMO_MODE:
            print "Scroll skip"
        else:
            print "Rendering full page. Scroll down ..."
            scroll = True
            lastheight = driver.execute_script("return " + scrollerjs)
            while scroll:
                driver.execute_script("window.scrollTo({left: 0, top: " + scrollerjs + ", behavior: 'smooth'})")
                time.sleep(scrollpause)

                newheight = driver.execute_script("return " + scrollerjs)
                if newheight == lastheight:
                    scroll = False
                lastheight = newheight

        print "Finding recommendations"
        recovids = driver.find_elements_by_xpath("//ytd-rich-item-renderer[@class='style-scope ytd-rich-grid-renderer']")
        totalrecs = len(recovids)


        print "Found", totalrecs, "new recommendations"
        driver.execute_script("window.scrollTo(0, 0)")


        currndest = 0
        print "Killin", totalrecs, "recomendations"
        for i in xrange(totalrecs):
            recoitem = recovids[i]
            ActionChains(driver).move_to_element(recoitem).perform()

            try:
                menudots_clicker = recoitem.find_element_by_xpath(".//button[@id='button' and @class='style-scope yt-icon-button' and @aria-label='Action menu']")
                #enudots_clicker = recoitem.find_element_by_xpath(".//yt-icon-button[@id='button' and @class='dropdown-trigger style-scope ytd-menu-renderer']")
                #enudots_clicker = recoitem.find_element_by_xpath(".//ytd-menu-renderer[@class='style-scope ytd-rich-grid-video-renderer']")
            except NoSuchElementException:
                print "No Such Element Exception Caught. Inspect the element", i, "now."
                time.sleep(1)
                continue

            # Show the menu
            ActionChains(driver).move_to_element(menudots_clicker).click().perform()
            time.sleep(1)

            NOT_INTERESTED = 3
            DONT_RECOMMEND_CHANNEL = 4

            if DEMO_MODE:
                print "click 1. menu off"
                ActionChains(driver).move_to_element(menudots_clicker).click().perform()
            else:
                # Kill the reco
                dropdown = driver.find_elements_by_xpath("//ytd-popup-container[@class='style-scope ytd-app']")
                dropoptions = dropdown[0].find_elements_by_xpath(".//ytd-menu-service-item-renderer[@class='style-scope ytd-menu-popup-renderer']")
                if len(dropoptions) == 1:
                    # this is music reco. dont kill. menu off
                    ActionChains(driver).move_to_element(menudots_clicker).click().perform()
                    continue
                killswitch_clicker = dropoptions[NOT_INTERESTED].find_element_by_xpath(".//yt-formatted-string[@class='style-scope ytd-menu-service-item-renderer']")
                ActionChains(driver).move_to_element(killswitch_clicker).click().perform()
                currndest += 1
                totaldest += 1
                time.sleep(1)
        print "Killed", currndest, "recomendations"
        print "Killed", totaldest, "recommendations in total"

        if currndest == 0:
            print "No recommendations killed"
            nokills += 1
            wait += 5
            if nokills >= nokillsruntime:
                break
        else:
            nokills = 0
            wait = waitdef


except:
    driver.close()
    print "Removed", totaldest, "recommendations"
    print "Congarts, you have defeated Youtube and Google"
    print
    print "Current date and time:"
    print str(datetime.datetime.now())

    import sendmail
    m = "\r\n".join([
      "From: notuberecs@gmail.com",
      "To: supratik.dnit@gmail.com",
      "Subject: Removed %d recommendations" % totaldest,
      "",
      "Removed %d recommendations" % totaldest,
      "Congarts, you have defeated Youtube and Google",
      ])

    print "Sending email"
    sendmail.sendamail('supratik.dnit@gmail.com', m)

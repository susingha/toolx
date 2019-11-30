import smtplib

def sendamail(toaddrs, msg):
    username = 'notuberecs@gmail.com'
    password = 'welcomE12'
    fromaddr = username

    server_ssl = smtplib.SMTP_SSL('smtp.gmail.com', 465)
    server_ssl.ehlo()
    server_ssl.login(username, password)
    server_ssl.sendmail(fromaddr, toaddrs, msg)
    server_ssl.close()

def sendamail_test():
    toaddrs  = 'supratik.dnit@gmail.com'
    m = "\r\n".join([
      "From: notuberecs@gmail.com",
      "To: supratik.dnit@gmail.com",
      "Subject: Just a message",
      "",
      "Why, oh why"
      ])

    sendamail(toaddrs, m)

# sendamail_test()


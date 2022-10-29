# using SendGrid's Python Library
# https://github.com/sendgrid/sendgrid-python
import os
import datetime
from sendgrid import SendGridAPIClient
from sendgrid.helpers.mail import Mail

sendgrid_api_key = 'add-your-key'

sup_to       = 'supratik.dnit@gmail.com'
sup_from     = 'supratik.dnit@gmail.com'
sup_subject  = 'sup: python email alert'
sup_content  = """
    <h1>Heading One</h1>
    <h2>Heading Two</h2>
    <body>Normal Body</body>
              """


def sendamail(_to      = sup_to,
              _from    = sup_from,
              _subject = sup_subject,
              _content = sup_content):
    message = Mail(
        from_email   = _from,
        to_emails    = _to,
        subject      = _subject + ' ' + str(datetime.datetime.now()),
        html_content = _content)
    try:
        sg = SendGridAPIClient(sendgrid_api_key)
        response = sg.send(message)
        print(response.status_code)
        print(response.body)
        print(response.headers)
    except Exception as e:
        print(e.message)

sendamail(_subject='sup: Attractions script exit')

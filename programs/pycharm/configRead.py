import configparser

CONFIGFILE = "switch1.conf"

parser = configparser.ConfigParser()
parser.read(CONFIGFILE)

print parser.sections()
print parser.options('network')
print parser.options('login')

ipaddr = parser.get('network', 'ipaddr')
sshuser = parser.get('login', 'sshuser')
sshpass = parser.get('login', 'sshpass')
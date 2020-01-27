import configparser
CONFIGFILE = "switch1.conf"

config = configparser.ConfigParser()

config['network'] = {
    'ipaddr': '10.16.151.115'
}
config['login'] = {
    'sshuser': 'admin',
    'sshpass': 'Aerohive123',
}

with open(CONFIGFILE, 'w') as f:
    config.write(f)
    f.close()




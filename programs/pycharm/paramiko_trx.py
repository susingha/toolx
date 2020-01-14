import paramiko
import random
import time

def cli_execute_wait(cli, wait = .5):
    print cli
    remote_conn.send(cli)
    time.sleep(wait)
    output = remote_conn.recv(65535)
    print output

ipaddr   = "10.16.151.115"
username = "admin"
password = "Aerohive123"

tftpaddr = "134.141.50.33"
tftprdir = random.randint(1000, 9999)

remote_conn_pre = paramiko.SSHClient()
remote_conn_pre.set_missing_host_key_policy(paramiko.AutoAddPolicy())
remote_conn_pre.connect(ipaddr, port = 22, username = username, password = password, look_for_keys = False, allow_agent = False)
remote_conn = remote_conn_pre.invoke_shell()
output = remote_conn.recv(65535)
print output

cli_execute_wait("show hivemanager status\n")
cli_execute_wait("show hivemanager address\n")
cli_execute_wait("application stop hiveagent\n", 2)

remote_conn_pre.close()

print "Connection Closed"
##### RECONNECT #######
print "Connecting Again"

remote_conn_pre = paramiko.SSHClient()
remote_conn_pre.set_missing_host_key_policy(paramiko.AutoAddPolicy())
remote_conn_pre.connect(ipaddr, port = 22, username = username, password = password, look_for_keys = False, allow_agent = False)
remote_conn = remote_conn_pre.invoke_shell()
output = remote_conn.recv(65535)
print output

cli_execute_wait("show hivemanager status\n")
cli_execute_wait("show hivemanager address\n")
cli_execute_wait("copy tftp://" + tftpaddr + "/Aerohive_b/Aerohive/hiveagent_pr   nvram:application hiveagent_pr\ny\n", 30)
cli_execute_wait("copy tftp://" + tftpaddr + "/Aerohive_b/Aerohive/hiveagent_pr   nvram:application hiveagent_pr_s\ny\n", 30)
cli_execute_wait("copy tftp://" + tftpaddr + "/Aerohive_b/Aerohive/hiveagent      nvram:application hiveagent\ny\n", 30)
cli_execute_wait("copy tftp://" + tftpaddr + "/Aerohive_b/Aerohive/ah_ha.conf     nvram:application ah_ha.conf\ny\n", 30)
cli_execute_wait("copy tftp://" + tftpaddr + "/Aerohive_b/Aerohive/ah_ha.conf     nvram:application ah_ha.conf_s\ny\n", 30)
cli_execute_wait("configure\n", 1)
cli_execute_wait("application install hiveagent start-on-boot\n", 1)
cli_execute_wait("exit\n", 1)
cli_execute_wait("copy system:running-config nvram:startup-config\ny\n", 45)
cli_execute_wait("application start hiveagent\n")

for i in range(5):
    time.sleep(5)
    cli_execute_wait("show hivemanager status\n")

remote_conn_pre.close()

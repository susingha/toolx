
print "beg"

ipaddr = "134.141.50.33"
usernm = "susingha"
passwd = "aerohive"

import base64
import paramiko

# the base64 is the host key of the ssh server as found in the known hosts file of the client
key = paramiko.ECDSAKey(data=base64.b64decode(b'AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBD7CedKyt5CqadgHv6CyfiNBeRY5suIz7smsrI93caNY/ciCOlrWrGT3o4KDHrVhg3YiiptloE8OXbfb1J9DcvM='))
#ey = paramiko.RSAKey(data=base64.b64decode(b'AAAAE2VjZHNhLXNoYTItbmlzdHAyNTYAAAAIbmlzdHAyNTYAAABBBD7CedKyt5CqadgHv6CyfiNBeRY5suIz7smsrI93caNY/ciCOlrWrGT3o4KDHrVhg3YiiptloE8OXbfb1J9DcvM='))

client = paramiko.SSHClient()
client.get_host_keys().add(ipaddr, 'ecdsa-sha2-nistp256', key)
#lient.get_host_keys().add(ipaddr, 'ssh-rsa', key)


client.connect(ipaddr, username=usernm, password=passwd)
stdin, stdout, stderr = client.exec_command('sudo ls -l')
for line in stdout:
    print(line.strip('\n'))
for line in stderr:
    print(line.strip('\n'))

stdin, stdout, stderr = client.exec_command('aerohive')
for line in stdout:
    print(line.strip('\n'))
for line in stdout:
    print(line.strip('\n'))
client.close()

print "end"
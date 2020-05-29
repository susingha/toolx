#!/usr/bin/python

import paramiko
import paramiko-expect
import random
import time


def get_login_handle(ipaddr, username, password):
    print "logging in:", ipaddr, username, password
    remote_conn_pre = paramiko.SSHClient()
    remote_conn_pre.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    remote_conn_pre.connect(ipaddr, port = 22, username = username, password = password, look_for_keys = False, allow_agent = False)
    remote_conn = remote_conn_pre.invoke_shell()
    output = remote_conn.recv(65535)
    print output
    return remote_conn, remote_conn_pre


def cli_execute_wait(conn, cli, wait = .5):
    print cli
    conn.send(cli)
    time.sleep(wait)
    output = conn.recv(65535)
    print output


print "ok"
for line in open("target.txt", 'r'):
    parts = line.split(',')
    ip   = parts[0].strip()
    user = parts[1].strip()
    pasw = parts[2].strip()
    
    sshconn, sshconn_pre = get_login_handle(ip, user, pasw)

    commandsfile = parts[3].strip()
    for cmdline in open(commandsfile, 'r'):
	cli_execute_wait(sshconn, cmdline, 1)

    sshconn_pre.close()

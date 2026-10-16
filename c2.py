#!/bin/python3

import socket
import argparse
import threading

MAX_BUF_SIZE = 2048

class Connection:
    def __init__(self, sockfd, ip, port):
        self.sockfd = -1
        self.ip = ""
        self.port = 0

def main(ip:str, port:int):
    threads = []
    thread_listener = threading.Thread(target=thread_listener, args=(client))
    thread_sender = threading.Thread(target=thread_sender, args=(client))
    while(1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((ip, port))
        s.listen()
        conn, addr = s.accept()
        ip, port = addr
        client = Connection(conn, ip, port)

        while(1):
            data = client.conn.recv(MAX_BUF_SIZE)
            if not data:
                break

def thread_listener(client:Connection):

def thread_sender(client:Connection):





def do_parse()->tuple:
    parser = argparse.ArgumentParser()
    parser.add_argument("ip", default="127.0.0.1", type=str)
    parser.add_argument("port", default=9090, type=int)
    parser.parse_args()
    return ip, port


if __name__ == "__main__":
    ip, port = do_parse()
    main(ip, port)
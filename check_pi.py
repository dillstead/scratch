# python3 ./check_pi.py 192.168.1.1 192.168.1.255 22
from ipaddress import IPv4Address
import socket
import sys

def is_listening(ip_addr, port):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(1)
        print(f"Trying  {ip_addr}:{port}...")
        result = sock.connect_ex((ip_addr, port))
        if result == 0:
            print(f"Server is listening on {ip_addr}:{port}")
    except socket.error as e:
        print(f"Error: {e}")
    finally:
        sock.close()
    return False

def scan_addresses(start_ip_addr, end_ip_addr, port):
    si = int(IPv4Address(start_ip_addr))
    ei = int(IPv4Address(end_ip_addr))

    if si > ei:
        print("Error: The first IP address must be smaller than the second IP address.")
        return

    cur = si
    while cur <= ei:
        if is_listening(str(IPv4Address(cur)), port):
            break
        cur += 1

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python check_pi <start_ip> <end_ip> <port>")
        sys.exit(1)

    start_ip_addr = sys.argv[1]
    end_ip_addr = sys.argv[2]
    port = int(sys.argv[3])

    scan_addresses(start_ip_addr, end_ip_addr, port)

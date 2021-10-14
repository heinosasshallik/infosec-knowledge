from autorecon.plugins import ServiceScan
from autorecon.io import error, info, fformat
from shutil import which
import os

class DnsReconDefault (ServiceScan):
        def __init__(self):
                super().__init__()
                self.name = "DnsRecon Default Scan"
                self.slug = 'dnsrecon'
                self.priority = 0
                self.tags = ['default', 'safe', 'dns']

        def configure(self):
                self.match_service_name('^domain')

        def check(self):
                tool = 'dnsrecon'
                if which('gobuster') is None:
                        error('The program dnsrecon could not be found. Make sure it is installed. (On Kali, run: sudo apt install dnsrecon)')

        def manual(self, service, plugin_was_run):
                service.add_manual_command('Use dnsrecon to automatically query data from the DNS server. You must specify the target domain name.', [
                        'dnsrecon -n {address} -d <DOMAIN-NAME> | tee {scandir}/{protocol}_{port}_dnsrecon_default_manual.txt'
                ])

        async def run(self, service):
                if self.get_global('domain'):
                        await service.execute('dnsrecon -n {address} -d ' + self.get_global('domain'), outfile='{protocol}_{port}_dnsrecon_default.txt')
                else:
                        await service.execute('echo "Domain name was not specified in the command line options. If you know the domain name, then look in the manual commands file for the dnsrecon command."', outfile='{scandir}/{protocol}_{port}_dnsrecon_default.txt')
                        
class DnsReconSubdomainBruteforce (ServiceScan):
        def __init__(self):
                super().__init__()
                self.name = "DnsRecon Bruteforce Subdomains"
                self.slug = 'dnsrecon-brute'
                self.priority = 0
                self.tags = ['default', 'safe', 'long', 'dns']

        def configure(self):
                self.match_service_name('^domain')

        def check(self):
                tool = 'dnsrecon'
                if which('gobuster') is None:
                        error('The program dnsrecon could not be found. Make sure it is installed. (On Kali, run: sudo apt install dnsrecon)')

        def manual(self, service, plugin_was_run):
                domain_name = '<DOMAIN-NAME>'
                if self.get_global('domain'):
                    domain_name = self.get_global('domain')
                service.add_manual_command('Use dnsrecon to bruteforce subdomains of a DNS domain.', [
                        'dnsrecon -n {address} -d ' + domain_name + ' -D /usr/share/seclists/Discovery/DNS/subdomains-top1million-110000.txt -t brt | tee {scandir}/{protocol}_{port}_dnsrecon_subdomain_bruteforce.txt',
                ])

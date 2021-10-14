from autorecon.plugins import ServiceScan
from autorecon.io import error, info, fformat
from shutil import which
import os

class DirBusterCustom(ServiceScan):

        def __init__(self):
                super().__init__()
                self.name = "Custom Directory Buster"
                self.slug = 'dirbuster-manual-extensions'
                self.priority = 0
                self.tags = ['default', 'safe', 'long', 'http']

        def configure(self):
                self.add_choice_option('tool', default='gobuster', choices=['feroxbuster', 'gobuster', 'dirsearch', 'ffuf', 'dirb'], help='The tool to use for directory busting. Default: %(default)s')
                self.add_list_option('wordlist', default=['/home/x90slide/resources/infosec-knowledge/wordlists/web_content/combined_words.txt'], help='The wordlist(s) to use for the custom HTTP scan plugin. Default: %(default)s')

                self.default_threads = 10
                self.default_ext = 'txt,html,php,asp,aspx,jsp'

                self.match_service_name('^http')
                self.match_service_name('^nacn_http$', negative_match=True)

        def check(self):
                tool = self.get_option('tool')
                if tool == 'feroxbuster':
                        if which('feroxbuster') is None:
                                error('The feroxbuster program could not be found. Make sure it is installed. (On Kali, run: sudo apt install feroxbuster)')
                elif tool == 'gobuster':
                        if which('gobuster') is None:
                                error('The gobuster program could not be found. Make sure it is installed. (On Kali, run: sudo apt install gobuster)')
                elif tool == 'dirsearch':
                        if which('dirsearch') is None:
                                error('The dirsearch program could not be found. Make sure it is installed. (On Kali, run: sudo apt install dirsearch)')

        def manual(self, service, plugin_was_run):
                dot_extensions = ','.join(['.' + x for x in self.default_ext.split(',')])
                for wordlist in self.get_option('wordlist'):
                        name = os.path.splitext(os.path.basename(wordlist))[0]
                        if self.get_option('tool') == 'feroxbuster':
                                service.add_manual_command('Enumerate files with extensions manually (change the extensions you want to enumerate).', ['feroxbuster -u {http_scheme}://{addressv6}:{port}/ -t ' + str(self.default_threads) + ' -w ' + wordlist + ' -x "' + self.default_ext + '" -v -k -n -q -o "{scandir}/{protocol}_{port}_{http_scheme}_feroxbuster_' + name + '.txt"'])
                        elif self.get_option('tool') == 'gobuster':
                                service.add_manual_command('Enumerate files with extensions manually (change the extensions you want to enumerate).', ['gobuster dir -u {http_scheme}://{addressv6}:{port}/ -t ' + str(self.default_threads) + ' -w ' + wordlist + ' -e -k -x "' + self.default_ext + '" -z -o "{scandir}/{protocol}_{port}_{http_scheme}_gobuster_' + name + '.txt"'])
                        elif self.get_option('tool') == 'dirsearch':
                                if service.target.ipversion == 'IPv6':
                                        error('dirsearch does not support IPv6.')
                                else:
                                        service.add_manual_command('Enumerate files with extensions manually (change the extensions you want to enumerate).', ['dirsearch -u {http_scheme}://{address}:{port}/ -t ' + str(self.default_threads) + ' -e "' + self.default_ext + '" -f -q -w ' + wordlist + ' --format=plain -o "{scandir}/{protocol}_{port}_{http_scheme}_dirsearch_' + name + '.txt"'])
                        elif self.get_option('tool') == 'ffuf':
                                service.add_manual_command('Enumerate files with extensions manually (change the extensions you want to enumerate).', ['ffuf -u {http_scheme}://{addressv6}:{port}/FUZZ -t ' + str(self.default_threads) + ' -w ' + wordlist + ' -e "' + dot_extensions + '" -v -noninteractive | tee {scandir}/{protocol}_{port}_{http_scheme}_ffuf_' + name + '.txt'])
                        elif self.get_option('tool') == 'dirb':
                                service.add_manual_command('Enumerate files with extensions manually (change the extensions you want to enumerate).', ['dirb {http_scheme}://{addressv6}:{port}/ ' + wordlist + ' -l -r -S -X ",' + dot_extensions + '" -o "{scandir}/{protocol}_{port}_{http_scheme}_dirb_' + name + '.txt"'])


class GobusterCommon(ServiceScan):
        def __init__(self):
                super().__init__()
                self.name = "Gobuster using common.txt with file extensions"
                self.slug = 'gobuster-common'
                self.priority = 0
                self.tags = ['default', 'safe', 'long', 'http']

        def configure(self):
                self.default_threads = 10
                self.default_ext = 'txt,html,php,asp,aspx,jsp'

                self.match_service_name('^http')
                self.match_service_name('^nacn_http$', negative_match=True)

        def check(self):
                tool = 'gobuster'
                if which('gobuster') is None:
                        error('The gobuster program could not be found. Make sure it is installed. (On Kali, run: sudo apt install gobuster)')


        async def run(self, service):
                wordlist = "/usr/share/seclists/Discovery/Web-Content/common.txt"
                name = os.path.splitext(os.path.basename(wordlist))[0]
                await service.execute('gobuster dir -u {http_scheme}://{addressv6}:{port}/ -t ' + str(self.default_threads) + ' -w ' + wordlist + ' -e -k -x "' + self.default_ext + '" -z -o "{scandir}/{protocol}_{port}_{http_scheme}_gobuster_' + name + '.txt"')


class CustomWebSubdomainEnumeration(ServiceScan):
        def __init__(self):
                super().__init__()
                self.name = "Custom Web Subdomain Enumerator"
                self.slug = 'custom-web-subdomain-enumerator'
                self.priority = 0
                self.tags = ['default', 'safe', 'long', 'http']

        def configure(self):
                self.match_service_name('^http')
                self.match_service_name('^nacn_http$', negative_match=True)

        def check(self):
                if which('ffuf') is None:
                        error('The ffuf program could not be found. Make sure it is installed. (On Kali, run: sudo apt install ffuf)')

        def manual(self, service, plugin_was_run):
                service.add_manual_command('(ffuf) Enumerate subdomains of a web server (you will probably have to filter out incorrect entries and change the HOST header)', ['ffuf -w /usr/share/seclists/Discovery/DNS/subdomains-top1million-110000.txt -H "HOST:FUZZ.{address}" -u {http_scheme}://{address}:{port} -s 2>&1 | tee {scandir}/{protocol}_{port}_{http_scheme}_ffuf_enumerate_subdomain.txt'])          

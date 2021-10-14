These are the custom configurations and plugins I use with AutoRecon. They belong in the `/root/.config/AutoRecon/` directory (because autorecon is usually run as **root**).

## Changes

* Added some more gobuster scans
  * Manual scan for separate file extensions scan
  * Quick common.txt scan with file extensions
  * DB backups scan
* Added a manual scan for subdomain enumeration of a website
* Added dnsrecon scans

## Additions

* I've made better wordlists to use for scanning.
  * Wordlists available here:
https://github.com/heinosasshallik/infosec-knowledge/tree/master/wordlists/web_content

## Usage

Suggested usage command:

```
sudo autorecon TARGET_HOSTNAME \
  --single-target \
  --output autorecon \
  --dirbuster.tool gobuster \
  --dirbuster.wordlist "/home/x90slide/resources/infosec-knowledge/wordlists/web_content/combined_directories.txt" \
  --dirbuster.ext "" \
  --dirbuster-manual-extensions.wordlist "/home/x90slide/resources/infosec-knowledge/wordlists/web_content/combined_words.txt"
```


## Installation

You need to have my custom plugins in autorecon's plugins directory. So either copy them over or create a symlink (useful for development)

Copy:

```
sudo cp /home/x90slide/resources/infosec-knowledge/scripts/autorecon_profiles/http_server_custom.py  /root/.config/AutoRecon/plugins/http_server_custom.py
sudo cp /home/x90slide/resources/infosec-knowledge/scripts/autorecon_profiles/dnsrecon_custom.py  /root/.config/AutoRecon/plugins/dnsrecon_custom.py
```

Symlink:

```
sudo ln -s /home/x90slide/resources/infosec-knowledge/scripts/autorecon_profiles/http_server_custom.py  /root/.config/AutoRecon/plugins/http_server_custom.py
sudo ln -s /home/x90slide/resources/infosec-knowledge/scripts/autorecon_profiles/dnsrecon_custom.py  /root/.config/AutoRecon/plugins/dnsrecon_custom.py
```

If that directory doesn't exist yet, then just run AutoRecon once as root, and they will be generated.

These are the custom configurations and plugins I use with AutoRecon. They belong in the `/root/.config/AutoRecon/` directory (because autorecon is usually run as **root**).

## TODO

Port over gobuster-common, gobuster-db-backups and the subdomain enumeration script to AutoRecon v2 plugin

## Changes

* Added some more gobuster scans
  * Manual scan for separate file extensions scan
  * Quick common.txt scan with file extensions
  * DB backups scan
* Added a manual scan for subdomain enumeration of a website

## Additions

* I've made better wordlists to use for scanning.
  * Wordlists available here:
https://github.com/heinosasshallik/infosec-knowledge/tree/master/wordlists/web_content


## Installation

You need to have my custom plugins in autorecon's plugins directory. So either copy them over or create a symlink (useful for development)

Copy:

```
cp /home/x90slide/resources/infosec-knowledge/scripts/autorecon_profiles/http_server_custom.py  /root/.config/AutoRecon/plugins/http_server_custom.py
```

Symlink:

```
ln -s /home/x90slide/resources/infosec-knowledge/scripts/autorecon_profiles/http_server_custom.py  /root/.config/AutoRecon/plugins/http_server_custom.py
```

If that directory doesn't exist yet, then just run AutoRecon once as root, and they will be generated.

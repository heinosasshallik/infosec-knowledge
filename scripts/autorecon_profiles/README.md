These are the profiles I use with AutoRecon. The profiles are located in the `~/.config/AutoRecon/` directory.

Note: **Do not forget that you usually run AutoRecon as root**, so don't forget to change the profiles in `/root/.config/AutoRecon/`, too.

## Changes

* Replaced ffuf web fuzzing with gobuster fuzzing
* Added some more gobuster scans and replaced the wordlists with my own combined wordlists 
  * Wordlists available here:
https://github.com/heinosasshallik/infosec-knowledge/tree/master/wordlists/web_content
* Added a manual scan for subdomain enumeration of a website

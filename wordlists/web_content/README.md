I've combined multiple wordlists into one, so I don't have to run so many scans to find everything. The wordlists are from SecLists (roughly in this order):

    * Directories:
      * raft-small-directories.txt
      * raft-small-directories-lowercase.txt
      * directory-list-2.3-small.txt
      * raft-medium-directories-lowercase.txt
      * raft-medium-directories.txt
      * directory-list-2.3-medium.txt
      * apache.txt 
      * common-words.txt (aka all the wordlists under "Files". Otherwise there would be no .git...)
      * directory-list-2.3-big.txt
      * raft-large-directories-lowercase.txt
      * raft-large-directories.txt 
      * directory-list-1.0.txt
    * Files:
      * common.txt
      * raft-small-words.txt
      * raft-small-words-lowercase.txt
      * raft-medium-words-lowercase.txt
      * raft-medium-words.txt
      * big.txt
      * raft-large-words-lowercase.txt
      * raft-large-words.txt          

Unique lines sorted out with (leave 1 occurrence, preserve ordering, keep the first occurrence):

```
cat file.txt | awk '! seen[$0]++'
```

Full commands for generating:

```
cat common.txt raft-small-words* raft-medium-words* big.txt raft-large-words* | awk '! seen[$0]++' > ~/resources/infosec-knowledge/wordlists/web_content/combined_words.txt

cat raft-small-directories* directory-list-2.3-small.txt raft-medium-directories* directory-list-2.3-medium.txt apache.txt ~/resources/infosec-knowledge/wordlists/web_content/combined_words.txt directory-list-2.3-big.txt raft-large-directories* directory-list-1.0.txt | awk '! seen[$0]++' > ~/resources/infosec-knowledge/wordlists/web_content/combined_directories.txt
```


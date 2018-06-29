# Node Vulnerability Digger
Finds vulnerable packages using npm audit. Then, it digs through the codebase to find usage of said vulnerable packages using grep.

Usage:
```bash ./node_vuln_digger.sh ./directory/where/package.json/is```
# nseq
Secret special project

---

Git fundamentals

git fetch - retrieves changes on remote branch 
git pull - puts those changes in your local copy of the branch

git merge - merges an iteration of the code into yours

git add <filename> adds a file changed to stage

git add . - adds all current files changed to stage

git commit -m "some message" - creates a commit with "some message"

git push - pushes commit to local branch, IF no remote origin set:
git push -u origin <some branch> - sets the upstream remote branch

Some aliases to add to your .gitconfig, just copy and paste this in
```bash
; Aliases
[alias]
	lazy = "!f() { git add . && git commit -m \"$@\" && git push; }; f"
[alias]
	mainfp = "!f() { git checkout main && git fetch && git pull; }; f"
[alias]
	mfp = "!f() { git mainfp; }; f"
[alias]
	fpull = "!f() { git fetch && git pull; }; f"
[alias]
	fp = "!f() { git fpull; }; f"
[alias]
	st = "!f() { git status -sb; }; f"
[alias]
	co = checkout
[alias]
	comp = "!f() { git difftool -t bc4 --no-prompt; }; f"
    
; this is a subcommand 
[alias]
	sub = submodule update --init --recursive
[diff]
	tool = bc3
[difftool "bc3"]
	path = c:/Program Files/Beyond Compare 4/bcomp.exe
[merge]
	tool = bc3
[mergetool "bc3"]
	path = c:/Program Files/Beyond Compare 4/bcomp.exe
```
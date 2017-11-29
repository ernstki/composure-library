git_show_dirty_repos ()
{
    author 'Kevin Ernst'
    about 'Show repos within subdirs of c.w.d. with uncommitted changes'
    param ''
    example ''
    group 'git'
    lsd | parallel cd {} \; test -d .git '&&' if [[ '$(git status --porcelain)' ]] \; then echo {} \; fi
}

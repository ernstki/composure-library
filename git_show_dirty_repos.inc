git_show_dirty_repos ()
{
    author 'Kevin Ernst'
    about 'Show repos within subdirs of c.w.d. with uncommitted changes'
    param ''
    example ''
    group 'git'
    lsd | parallel cd {} \; test -d .git '&&' if git diff --no-ext-diff --quiet --exit-code \; then echo {} \; fi
}

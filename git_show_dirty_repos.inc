git_show_dirty_repos ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    lsd | parallel cd {} \; test -d .git '&&' if [[ '$( git status --porcelain )' ]] \; then echo {} \; fi
}

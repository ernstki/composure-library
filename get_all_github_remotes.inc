get_all_github_remotes ()
{
    author 'Kevin Ernst'
    about 'fetches all remotes from a bunch of Git-controlled subdirectories'
    param ''
    example ''
    group 'git'
    lsd | parallel cd {} '&&' git remote -v '|' sed -n "'s#.*://\(.*\) (fetch)#\1#p'"
}

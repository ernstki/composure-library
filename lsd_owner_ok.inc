lsd_owner_ok ()
{
    author 'Kevin Ernst'
    about 'List directories that are owned by the user the same as their name'
    param '(none)'
    example ''
    group 'filesystem'
    find . -type d -maxdepth 1 -ls | awk '"./",$5 = $11 { print $5 }' | sed 's#^\./##'
}

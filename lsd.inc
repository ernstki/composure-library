lsd ()
{
    author 'Kevin Ernst <ernstki -at- mail.uc.edu>'
    about 'List only directories in the c.w.d.'
    param 'none'
    example ''
    group 'filesystem'
    find -mindepth 1 -maxdepth 1 -type d | sed 's#^\./##'
}

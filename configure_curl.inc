configure_curl ()
{
    author 'Kevin Ernst <ernstki -at- mail.uc.edu>'
    about 'Specify appropriate `configure` options to build libcurl from source'
    param ''
    example ''
    group 'build'
    ./configure --prefix=$HOME/opt --with-ssl=/home/ajjfr/opt/ssl/ --with-ca-bundle=$HOME/opt/share/curl/curl-ca-bundle.crt
}

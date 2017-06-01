github_query_latest_release ()
{
    author 'Kevin Ernst'
    about 'Query the tarball URL for the latest release of a given GitHub project'
    param '$1: username/reponame of the desired project'
    example '$ github_query_latest_release erichs/composure'
    group 'github'
    curl -s https://api.github.com/repos/$1/releases/latest | sed -n 's/.*"tarball_url": "\([^"][^"]*\)".*/\1/p'
}

touch_subdirs_with ()
{
    author 'Kevin Ernst'
    about 'Update modification time of subdirs based on a file they all contain'
    param '$1 - a file which exists within each subdir (e.g., "sub_name.txt")'
    example 'touch_subdirs_with job_properties.txt'
    group 'tftools'
    lsd | parallel cd {} \; touch -r "'$1'" . '||' echo '"{}: failed"'
}

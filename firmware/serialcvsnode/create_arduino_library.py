import shutil
import os

# https://arduino.github.io/arduino-cli/latest/library-specification/

#
# Some Constants
library_name = 'embedded-serial-csv'

#
# Get the current Path of the script
dirname = os.path.dirname(__file__)
dest = os.path.join(dirname, library_name)
print('The destination folder is ', dest)

#
# Copy the ETL to the Arduino-Library Folder
src = os.path.join(dirname, 'externals/etl/include')
shutil.copytree(src, os.path.join(dest, 'src'),  dirs_exist_ok=True)
print('Done adding ETL...')

#
# Add files from the src folder
src = os.path.join(dirname, 'src')
shutil.copytree(src, os.path.join(dest, 'src'), dirs_exist_ok=True)

#
# Add any additional file (library.properties)
filename = 'library.properties'
shutil.copyfile(os.path.join(dirname, filename), os.path.join(dest, filename))

configure_file(AppxManifest.xml
               ${BUILD_PATH}/AppxManifest.xml COPYONLY)
configure_file(../linux/icons/128x128/apps/${APP_ID}.png
               ${BUILD_PATH}/icon.png COPYONLY)


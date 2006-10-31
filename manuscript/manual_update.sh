#!bin/bash
mv Manual.pages Manual4.pages
svn up
cp -R Manual.pages/thumbs/.svn Manual4.pages/thumbs/
cp -R Manual.pages/Contents/.svn Manual4.pages/Contents/
cp -R Manual.pages/.svn Manual4.pages/
rm -rf Manual.pages
mv Manual4.pages Manual.pages
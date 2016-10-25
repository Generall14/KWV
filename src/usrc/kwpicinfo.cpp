#include "kwpicinfo.h"

KWPicInfo::KWPicInfo()
{
    this->Clear();
}

void KWPicInfo::Clear()
{
    fileInfo = QFileInfo();
    isGif = false;
    frames = -1;
    frameDelay = -1;
    resWidth = -1;
    resHeight = -1;
    picDepth = -1;
    loadTimeUs = -1;
}

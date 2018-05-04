#include "EWTheme.h"

EWTheme::EWTheme(const char* readFileName,const char* writeFileName)
{
    this->readFileName = readFileName;
    this->writeFileName = writeFileName;

    d = new EWDataSource();
    m = new EWModel();
}

EWTheme::~EWTheme()
{
    if(d != NULL)
    {
        delete d;
        d = NULL;
    }

    if(m != NULL)
    {
        delete m;
        m = NULL;
    }
}

void EWTheme::RunModel()
{
    d->Read_File(readFileName);

    m->EWProcess(d->data);

    d->Output_File(writeFileName,m->score);
}

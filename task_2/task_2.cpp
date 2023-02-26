#include <iostream>
#include "observer.h"

int main()
{
    Observed subj;
    WarningObserver wo1(&subj), wo2(&subj), wo3(&subj);
    ErrorObserver eo1(&subj), eo2(& subj);
    FatalErrorObserver feo(&subj);
    subj.warning("warning 1");
    subj.error("error 1");
    subj.fatalError("fatalError 1");
}


#ifndef AAOBJ_H
#define AAOBJ_H

#include "aaform.h"
#include "ui_aaform.h"
#include "amber_base_inc.h"

class AAObj
{
public:

    enum class AAType {
        ADD_OFFSET         ,
        ADD_VALUE          ,
        MULT_OFFSET        ,
        MULT_VALUE         ,
        DEL_SV             ,
        DEL_BPM            ,
        CONV_SV            ,
        CONV_BPM           ,
        ADD_TPLIST         ,
        SUBTRACT_TPLIST    ,
        MULT_TPLIST        ,
        DIV_TPLIST         ,
        INVERT             ,
        LIMITVAL
    };

    AAObj(AAType newObjType);
    ~AAObj();

    void setForm();
    void showForm();
    void closeForm();

    void setFormDefault();
    void setObjType(AAType newObjType);

    cOM_TPList applyEffect(cOM_TPList oldTPList);

    AAType getEffect();
    QString getEffectName(bool getInfo = false);

protected:

    AAType objType;
    AAForm *ui;


};

#endif // AAOBJ_H

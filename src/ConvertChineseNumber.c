/*
 ============================================================================
 Name        : ConvertChineseNumber.c
 Author      : amwyga
 Version     : 0.1
 Copyright   : MIT
 Description :
 2019.1.13
             1.convert chinese character to number
             2.convert number to chinese character
 xx.xx.xx

 ============================================================================
 */

/********************************file header**********************************/
#include "types.h"
#include <stdlib.h>
#include <string.h>

/********************************MACRO define**********************************/
#define TEMP_BUFFER_LENGTH 256
#define CHINESE_CHAR_LEN 3
#define USE_SIMPLE  1

/********************************data structure define**********************************/
/* for chinese to number */
typedef struct
{
    const T_CHAR *name;/* chinese name */
    T_UINT32 value;    /* number */
    T_BOOL isSection; /* is section chinese name*/
    T_BOOL isUnit;
} T_ChineseNameValue;

/********************************local variable**********************************/
static const T_CHAR* m_simpleChineseNumberChar[10]={"零","一","二","三","四","五","六","七","八","九"};
static const T_CHAR* m_simpleChineseUnit[4]={"","十","百","千"};
static const T_CHAR* m_complexChineseNumberChar[10]={"零","壹","贰","叁","肆","伍","陆","柒","捌","玖"};
static const T_CHAR* m_complexChineseUnit[4]={"","拾","佰","仟"};
static const T_CHAR* m_chineseSection[3]={"零","万","亿"};

static const T_UINT32 m_chineseUnitLen[]={0,CHINESE_CHAR_LEN,CHINESE_CHAR_LEN,CHINESE_CHAR_LEN};
static const T_UINT32 m_chineseSectionLen[]={CHINESE_CHAR_LEN,CHINESE_CHAR_LEN,CHINESE_CHAR_LEN,CHINESE_CHAR_LEN * 2};

static const T_ChineseNameValue m_simpleChineseValuePair[]=
{
    {"零",0,FALSE,FALSE},
    {"一",1,FALSE,FALSE},
    {"二",2,FALSE,FALSE},
    {"三",3,FALSE,FALSE},
    {"四",4,FALSE,FALSE},
    {"五",5,FALSE,FALSE},
    {"六",6,FALSE,FALSE},
    {"七",7,FALSE,FALSE},
    {"八",8,FALSE,FALSE},
    {"九",9,FALSE,FALSE},
    {"十",10,FALSE,TRUE},
    {"百",100,FALSE,TRUE},
    {"千",1000,FALSE,TRUE},
    {"万",10000,TRUE,TRUE},
    {"亿",100000000,TRUE,TRUE},
    {NULL,0,FALSE,FALSE}
};

static const T_ChineseNameValue m_complexChineseValuePair[]=
{
    {"零",0,FALSE,FALSE},
    {"壹",1,FALSE,FALSE},
    {"贰",2,FALSE,FALSE},
    {"叁",3,FALSE,FALSE},
    {"肆",4,FALSE,FALSE},
    {"伍",5,FALSE,FALSE},
    {"陆",6,FALSE,FALSE},
    {"柒",7,FALSE,FALSE},
    {"捌",8,FALSE,FALSE},
    {"玖",9,FALSE,FALSE},
    {"拾",10,FALSE,TRUE},
    {"佰",100,FALSE,TRUE},
    {"仟",1000,FALSE,TRUE},
    {"万",10000,TRUE,TRUE},
    {"亿",100000000,TRUE,TRUE},
    {NULL,0,FALSE,FALSE}
};

#if USE_SIMPLE
static const T_CHAR** m_pChineseNumberChar = m_simpleChineseNumberChar;
static const T_CHAR** m_pChineseUnit = m_simpleChineseUnit;
static const T_ChineseNameValue *m_pChineseValuePair = m_simpleChineseValuePair;
#else
static const T_CHAR** m_pChineseNumberChar = m_complexChineseNumberChar;
static const T_CHAR** m_pChineseUnit = m_complexChineseUnit;
static const T_ChineseNameValue *m_pChineseValuePair = m_complexChineseValuePair;
#endif

/***********************************implemention*********************************************/
/**
 * @brief
 *  insert a chinese character to chinese string header
 * @param[in|out]:chineseStr chinese string
 * @param[in]:chineseStrLen: chinese string length
 * @param[in]:insertStr: insert chinese character
 * @param[in]:insertStrLen: insert chinese character length
 * @return
 *  none
 */
static void InsertStringToHeader(T_CHAR *chineseStr,T_UINT32 chineseStrLen,const T_CHAR *insertStr,T_UINT32 inserStrLen)
{
    T_SINT32 i = 0;
    for(i = chineseStrLen - 1; i > 0; i = i - 3){
        *(chineseStr + i + inserStrLen) = *(chineseStr + i);
        *(chineseStr + i + inserStrLen - 1) = *(chineseStr + i - 1);
        *(chineseStr + i + inserStrLen - 2) = *(chineseStr + i - 2);
    }

    memcpy(chineseStr,insertStr,inserStrLen);
}

/**
 *@brief
 *  convert a small number to chinese character
 *＠param[in] section:input number
 *＠param[out] chineseStr:chinese character
 *@return
 *    chinese charcter string length
 */
static T_UINT32 SectionNumberToChinese(T_UINT32 section,T_CHAR *chineseStr)
{
    T_CHAR tempBuffer[TEMP_BUFFER_LENGTH];
    T_CHAR *pTempBuffer = tempBuffer;
    T_UINT32 unitPos = 0;
    T_BOOL zero = TRUE;
    T_UINT32 chineseStrLen = 0;
    T_UINT32 len = 0;

    memset(tempBuffer,0,TEMP_BUFFER_LENGTH);
    while (section > 0)
    {
        T_UINT32 num = section % 10;
        if (0 == num)
        {
            if (0 == section || !zero)
            {
                zero = TRUE;
                InsertStringToHeader(chineseStr,chineseStrLen,m_pChineseNumberChar[num],CHINESE_CHAR_LEN);
                chineseStrLen += CHINESE_CHAR_LEN;
            }
        }else{
            zero = FALSE;
            memcpy(pTempBuffer,m_pChineseNumberChar[num],CHINESE_CHAR_LEN);
            len = CHINESE_CHAR_LEN;

            memcpy(tempBuffer + len,m_pChineseUnit[unitPos],m_chineseUnitLen[unitPos]);
            len += m_chineseUnitLen[unitPos];

            InsertStringToHeader(chineseStr,chineseStrLen,tempBuffer,len);
            chineseStrLen += len;
        }

        unitPos++;
        section = section / 10;
    }

    return chineseStrLen;
}

/**
 *@brief
 *  convert number to chinese character
 *＠param[in] number:input number
 *＠param[out] chineseStr:chinese character
 *@return
 *    TRUE:success,chineseStr is valid
 *    FALSE:fail,chineseStr is invalid
 */
static T_BOOL NumberToChinese(T_UINT64 number,T_CHAR *chineseStr)
{
    T_SINT32 unitPos = 0;
    T_CHAR tempBuffer[TEMP_BUFFER_LENGTH];
    T_BOOL isNeedZero = FALSE;
    T_SINT32 chineseStrLen = 0;
    T_UINT32 len = 0;
    T_UINT32 i = 0;

    if (NULL == chineseStr){
        return FALSE;
    }

    while(number > 0){
        T_UINT32 section = (T_UINT32)(number % 10000);
        if (TRUE == isNeedZero)
        {
            InsertStringToHeader(chineseStr,chineseStrLen,m_pChineseNumberChar[0],CHINESE_CHAR_LEN);
            chineseStrLen += CHINESE_CHAR_LEN;
        }

        len = SectionNumberToChinese(section,tempBuffer);
        if (0 == section && unitPos != 0){
            memcpy(tempBuffer + len,m_chineseSection[0],m_chineseSectionLen[0]);
            len += m_chineseSectionLen[0];
        }else {
            if (unitPos > 2){
                if (unitPos % 2 == 1){
                    memcpy(tempBuffer + len,m_chineseSection[1],m_chineseSectionLen[1]);
                    len += m_chineseSectionLen[1];
                }

                for(i = 0; i < unitPos / 2; i++){
                    memcpy(tempBuffer + len,m_chineseSection[2],m_chineseSectionLen[2]);
                    len += m_chineseSectionLen[2];
                }
            }else if (unitPos != 0){
                memcpy(tempBuffer + len,m_chineseSection[unitPos],m_chineseSectionLen[unitPos]);
                len += m_chineseSectionLen[unitPos];
            }
        }

        InsertStringToHeader(chineseStr,chineseStrLen,tempBuffer,len);
        chineseStrLen += len;

        isNeedZero = (section < 1000 && section > 0);
        number = number / 10000;

        unitPos++;
    }

    return TRUE;
}

/********************************Convert Chinese To Number *****************************/
/**
 * @brief
 * conver single chinese charcter to number
 * @param[in]: chineseStr:single chinese charcter
 * @param[in]: isUnit:the expect Chinese characters to be unit
 * @return
 *  <0:  error
 *  >=0: value
 */
static T_SINT32 ChineseToValue(const T_CHAR *chineseStr,T_BOOL isUnit)
{
    const T_ChineseNameValue *val=m_pChineseValuePair;
    while(NULL != (*val).name)
    {
        if (  (memcmp(chineseStr,(*val).name,CHINESE_CHAR_LEN) == 0)
           && (isUnit == (*val).isUnit))
        {
            return (*val).value;
        }

        val++;
    }

    return -1;
}

/**
 * brief:
 *  get chineseStr section status
 * @param[in]:chineseStr:chinese charcter
 * @param[out]:isSectinOver:current section is over
 *  TRUE:current section chinese charcter is over
 *  FALSE:current section chinese charcter is not over
 * @return
 *  TRUE:this is a section chinese charcter
 *  FALSE:this is not a section chinese charcter
 */
static T_BOOL GetSectionStatus(const T_CHAR *chineseStr,T_BOOL *isSectinOver){
    const T_ChineseNameValue *val=m_pChineseValuePair;
    T_BOOL result = FALSE;

    while(NULL != (*val).name)
    {
        if (  (memcmp(chineseStr,(*val).name,CHINESE_CHAR_LEN) == 0)
           && (TRUE == (*val).isSection))
        {
            result = TRUE;
            break;
        }

        val++;
    }

    if (result != FALSE){
        *isSectinOver = TRUE;
    }

    val=m_pChineseValuePair;
    while(NULL != (*val).name)
    {
        if (  (memcmp(chineseStr + CHINESE_CHAR_LEN,(*val).name,CHINESE_CHAR_LEN) == 0)
           && (TRUE == (*val).isSection))
        {
            *isSectinOver = FALSE;
            break;
        }

        val++;
    }

    return result;
}

/**
 *@brief
 *  convert chinese character to number
 *＠param[in] chineseStr:chinese character
 *＠param[in] number:output number
 *@return
 *    TRUE:success,output number is valid
 *    FALSE:fail,output number is invalid
 */
static T_BOOL ChineseToNumber(const T_CHAR *chineseStr,T_UINT64 *result)
{
    T_UINT64 unitNumber = 0;
    T_UINT64 sectionNumber = 0;
    T_BOOL isSectionOver = FALSE;
    T_UINT32 offset = 0;
    T_UINT32 chineseLen = 0;
    T_CHAR tempBuffer[TEMP_BUFFER_LENGTH];
    T_CHAR *pTempBuffer = tempBuffer;
    const T_CHAR *pChineseStr = chineseStr;

    if ((NULL == chineseStr) || ('\0' == *chineseStr) || (NULL == result)){
        return FALSE;
    }

    while(*pChineseStr){
        pChineseStr += CHINESE_CHAR_LEN;
    }

    chineseLen = pChineseStr - chineseStr;
    while(offset < chineseLen){
        memcpy(pTempBuffer,chineseStr + offset,CHINESE_CHAR_LEN);
        tempBuffer[CHINESE_CHAR_LEN] = '\0';
        if (offset + CHINESE_CHAR_LEN < chineseLen){
            memcpy(tempBuffer + CHINESE_CHAR_LEN ,chineseStr + offset + CHINESE_CHAR_LEN,CHINESE_CHAR_LEN);
            tempBuffer[CHINESE_CHAR_LEN * 2] = '\0';
        }

        T_SINT32 num = ChineseToValue(tempBuffer,FALSE);
        if (num >= 0){
            unitNumber = num;
            offset += CHINESE_CHAR_LEN;
            if (offset >= chineseLen){
                sectionNumber += unitNumber;
                *result += sectionNumber;
                break;
            }
        }else{
            T_SINT32 unit = ChineseToValue(tempBuffer,TRUE);
            T_BOOL isSection = GetSectionStatus(tempBuffer,&isSectionOver);
            if (unit < 0){
                return FALSE;
            }

            if (TRUE == isSection){
                sectionNumber = (sectionNumber + unitNumber) * unit;
                if (TRUE == isSectionOver){
                    *result += sectionNumber;
                    sectionNumber = 0;
                    isSectionOver = FALSE;
                }
            } else{
                sectionNumber += (unitNumber * unit);
            }

            unitNumber = 0;
            offset += CHINESE_CHAR_LEN;
            if (offset >= chineseLen){
                *result += sectionNumber;
                break;
            }
        }
    }

    return TRUE;
}

int main(void) {
    T_CHAR temp_buffer[TEMP_BUFFER_LENGTH];
    T_UINT64 value = 0;

    memset(temp_buffer,0,TEMP_BUFFER_LENGTH);

    NumberToChinese(12300404565557890123ULL,temp_buffer);
    printf("output1:%s",temp_buffer);

    ChineseToNumber("一千二百三十亿亿零四百零四万亿五千六百五十五亿五千七百八十九万零一百二十三",&value);
    printf("output2:%llu",value);

    return 0;
}

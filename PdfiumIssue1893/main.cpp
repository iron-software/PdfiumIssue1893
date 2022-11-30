#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <exception>
#include <fstream>
#include <map>
#include <stack>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include "fpdfview.h"
#include "fpdf_annot.h"
#include "fpdf_attachment.h"
#include "fpdf_formfill.h"
#include "fpdf_flatten.h"
#include "fpdf_edit.h"
#include "fpdf_ppo.h"
#include "fpdf_save.h"
#include "fpdf_signature.h"
#include "fpdf_text.h"
#include "fpdf_transformpage.h"
#include "direct.h"

using namespace std;

class PdfWriter : public FPDF_FILEWRITE
{
private:
    inline static int GetBlock(void* param, unsigned long pos, unsigned char* buf, unsigned long size)
    {
        int result = 0;
        try
        {
            if (!param || pos + size < pos)
                return 0;
            memcpy(buf, (char*)param + pos, size);
            result = 1;
        }
        catch (const std::exception& ex)
        {
            result = 0;
        }
        return result;
    }
    inline static int WriteBlockCallback(FPDF_FILEWRITE* pFileWrite, const void* data, unsigned long size)
    {
        if (data == nullptr || size < 1)
            return 1;
        int result = 0;
        try
        {
            //LOG << "Pdf document writing " << size << " bytes";
            PdfWriter* instance = static_cast<PdfWriter*>(pFileWrite);
            char* ptr = reinterpret_cast<char*>(const_cast<void*>(data));
            instance->bytes.insert(std::end(instance->bytes), ptr, ptr + size);
            result = 1;
        }
        catch (const std::exception& ex)
        {
            result = 0;
        }
        return result;
    }
    vector<char> bytes;
public:
    inline PdfWriter()
    {
        FPDF_FILEWRITE::version = 1;
        FPDF_FILEWRITE::WriteBlock = WriteBlockCallback;
    }
    inline void Clear()
    {
        this->bytes.clear();
    }
    inline vector<char> GetBytes()
    {
        return this->bytes;
    }
    inline FPDF_FILEACCESS GetFileAccessor(vector<char> data)
    {
        FPDF_FILEACCESS file_access = {};
        file_access.m_FileLen = data.size();
        file_access.m_GetBlock = PdfWriter::GetBlock;
        // Copy data to prevent clearing it before saved document close.
        this->bytes = data;
        file_access.m_Param = &this->bytes[0];
        return file_access;
    }
};

FPDF_PAGE workaround(FPDF_DOCUMENT doc, int page_index)
{
    FPDF_PAGE page_in = FPDF_LoadPage(doc, page_index);
    float width = FPDF_GetPageWidthF(page_in);
    float height = FPDF_GetPageHeightF(page_in);
    FS_MATRIX m;
    m.a = 1;
    m.b = 0;
    m.c = 0;
    m.d = 1;
    m.e = 0;
    m.f = 0;
    FS_RECTF rectf;
    rectf.bottom = 0;
    rectf.left = 0;
    rectf.right = width;
    rectf.top = height;
    FPDFPage_TransFormWithClip(page_in, &m, &rectf);
    FPDFPage_SetMediaBox(page_in, 0, 0, width, height);
    //
    FPDFPage_GenerateContent(page_in);
    FPDF_ClosePage(page_in);
    FPDF_PAGE page_out = FPDF_LoadPage(doc, page_index);
    return page_out;
}
int main()
{
    // params
    bool use_fix = false; // set to true to demonstrate the fix!
    int page_index = 1;
    int object_index = 107;
    // init library
    FPDF_LIBRARY_CONFIG config;
    config.version = 2;
    config.m_pUserFontPaths = NULL;
    config.m_pIsolate = NULL;
    config.m_v8EmbedderSlot = 0;
    FPDF_InitLibraryWithConfig(&config);
    // get working dir
    char buff[FILENAME_MAX]; //create string buffer to hold path
    _getcwd(buff, FILENAME_MAX);
    std::string dir = std::string(buff);
    std::string path = dir + "\\elvis.pdf";
    // open document
    FPDF_DOCUMENT handle = FPDF_LoadDocument(path.c_str(), "");
    if (handle == nullptr)
        throw std::exception("failed to load document");
    // load page (2nd page)
    FPDF_PAGE page = nullptr;
    if(use_fix)
    {
        // load page w/ workaround
        page = workaround(handle, page_index);
    }
    else
    {
        // load page
        page = FPDF_LoadPage(handle, page_index);
    }
    if (page == nullptr)
        throw std::exception("failed to load page");
    // load text page
    FPDF_TEXTPAGE text_page = FPDFText_LoadPage(page);
    if (text_page == nullptr)
        throw std::exception("failed to load text page");
    // get object (text that says "Presley")
    FPDF_PAGEOBJECT object = FPDFPage_GetObject(page, object_index);
    if (object == nullptr)
        throw std::exception("failed to retrieve object");
    // remove object
    FPDF_BOOL result = FPDFPage_RemoveObject(page, object);
    if (!result)
        throw std::exception("error while removing page object");
    FPDFPageObj_Destroy(object);
    // re-generate page content
    result = FPDFPage_GenerateContent(page);
    if (!result)
        throw std::exception("error while re-generating content");
    // close page
    FPDFText_ClosePage(text_page);
    FPDF_ClosePage(page);
    // re-generate document bytes
    PdfWriter writer;
    result = FPDF_SaveAsCopy(handle, &writer, 0);
    if (!result)
        throw std::exception("error while saving document");
    // save modified doc to disk
    vector<char> bytes_out = writer.GetBytes();
    ofstream fout;
    fout.open("result.pdf", ios::binary | ios::out);
    fout.write((char*)&bytes_out[0], sizeof(char) * bytes_out.size());
    fout.close();
    return 0;
}
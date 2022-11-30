#ifndef PUBLIC_IPDF_H_
#define PUBLIC_IPDF_H_
#ifdef LINUX
#define __stdcall
#endif
// NOTE 12/16/20: do not include any other pdfium types/headers here so this
// file can be included by other applications NOTE 12/16/20: non-pubic pdfium
// types/headers must be included within the implementation (.cpp) file
#include <public/fpdfview.h>
#include <vector>
#ifdef __cplusplus
extern "C" {
#endif
namespace Ipdf
{
/// <summary>
/// Default event handler with a string parameter
/// </summary>
typedef void(__stdcall* DefaultEvent)(const char*);
}
/// <summary>
/// Error codes
/// </summary>
enum IpdfErrorCodes
{
  NoError = 0,
  InvalidDocumentPointer = -1,
  InvalidFontName = -2,
  InvalidPageIndex = -3,
  InvalidPageObjectIndex = -4,
  PageObjectTypeMismatch = -5,
  InvalidPageCacheId = -6,
  InvalidFormFieldId = -7,
  InvalidAcroFormsDictionary = -8,
  FailedToLoadDocument = -9,
  InvalidDocumentId = -10,
  FailedToRetrieveStockFont = -11,
  EmptyFontName = -12,
  InvalidAnnotationsArray = -13,
  InvalidAcroFormsFieldsArray = -14,
  InvalidComboBoxOptionsArray = -15,
  InvalidPagePointer = -16,
  InvalidStreamContentsArray = -17,
  InvalidInfoDictionary = -18,
  InvalidParser = -19,
  InvalidEncryptionDictionary = -20,
  InvalidSecurityHandler = -21,
  InvalidOutlinesDictionary = -22,
  InvalidFirstBookmarkDictionary = -23,
  DocumentIdAlreadyTaken = -24,
  InvalidBookmarkIndex = -25,
  InvalidBookmarkDescriptors = -26,
  AnnotationCountMismatch = -27,
  InvalidAnnotationIndex = -28,
  InvalidRadioButtonKidsArray = -29,
  FailedToFindRadioButtonFieldName = -30
};
enum ImageColorSpaces
{
    UnknownColorSpace =0,
    Grayscale = 1,
    RGB = 2
};
enum ImageFilters
{
    UnknownFilter = 0,
    FlateEncode = 1,
    DCTEncode = 2,
    Multiple = 3,
};
struct IPDF_RESULT
{
    bool Success;
    IpdfErrorCodes ErrorCode;
};
/// <summary>
/// Font resource
/// </summary>
struct IPDF_FONT
{
  /// <summary>
  /// Font ID
  /// </summary>
  int ID;
};
/// <summary>
/// Radio button form field
/// </summary>
struct IPDF_RADIOBUTTONFIELD
{
    /// <summary>
    /// Form field ID
    /// </summary>
    int ID;
    /// <summary>
    /// Form field page index
    /// </summary>
    int PageIndex;
};
/// <summary>
/// Combobox form field
/// </summary>
struct IPDF_COMBOBOX
{
  /// <summary>
  /// Form field ID
  /// </summary>
  int ID;
  FS_RECTF Rect;
};
/// <summary>
/// Text form field
/// </summary>
struct IPDF_TEXTFORM
{
  /// <summary>
  /// Form field ID
  /// </summary>
  int ID;
  bool Multiline;
  bool Required;
  IPDF_FONT Font;
  FS_RECTF Rect;
};
struct IPDF_IMAGE
{
    int ObjNum;
    ImageColorSpaces ColorSpace;
    int BitsPerComponent;
    int Width;
    int Height;
    ImageFilters Filter;
    int DataLength;
    char* Data;
    int MaskWidth;
    int MaskHeight;
    int MaskDataLength;
    char* MaskData;
};
IPDF_RESULT IPDF_AddBookmark(FPDF_DOCUMENT doc, const wchar_t* title, const wchar_t* parent, const wchar_t* previous,
    int page_index, int type, int left, int right, int top, int bottom, int zoom, const wchar_t* chain);
IPDF_RESULT IPDF_AddBookmark2(FPDF_DOCUMENT doc, const wchar_t* title, const wchar_t* parenttree, const wchar_t* prevtitle,
    int destpageindex, int destpagefittype, int destleft, int desttop, int destzoom, int left, int bottom, int right, int top);
/// <summary>
/// Add a font to a document using the specified base font name
/// </summary>
/// <param name="document">Document to add the font to</param>
/// <param name="name">Base font name</param>
/// <param name="result">(Output) Result</param>
/// <returns>Font info</returns>
FPDF_EXPORT IPDF_FONT __stdcall IPDF_AddFont(FPDF_DOCUMENT document, const char* name, IPDF_RESULT& result);
FPDF_EXPORT IPDF_TEXTFORM __stdcall IPDF_AddFormCheckboxField(FPDF_DOCUMENT document, int page_index,
    FS_RECTF rect, IPDF_FONT font, unsigned int font_size, bool multiline, const wchar_t* field_name, IPDF_RESULT& result);
FPDF_EXPORT IPDF_RADIOBUTTONFIELD __stdcall IPDF_AddFormRadioButtonField(FPDF_DOCUMENT document, int page_index, bool required, const wchar_t* field_name, IPDF_RESULT& result);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_AppendRadioButtonOption(FPDF_DOCUMENT document, IPDF_RADIOBUTTONFIELD object, const char* option, FS_RECTF rect,
    IPDF_FONT font, unsigned int font_size);
/// <summary>
/// Add a form text field to a document using the specified attributes
/// </summary>
/// <param name="document">Document to add the form field to</param>
/// <param name="page_index">Page index</param>
/// <param name="rect">Form field area</param>
/// <param name="font">Font</param>
/// <param name="font_size">Font size</param>
/// <param name="multiline">Multiline text input</param>
/// <param name="required">Input is required</param>
/// <param name="field_name">Input field name</param>
/// <param name="result">(Output) result</param>
/// <returns>Text form</returns>
FPDF_EXPORT IPDF_TEXTFORM __stdcall IPDF_AddFormTextField(FPDF_DOCUMENT document, int page_index,
    FS_RECTF rect, IPDF_FONT font, unsigned int font_size, bool multiline, bool required, const wchar_t* field_name, IPDF_RESULT& result);
/// <summary>
/// Add a form combobox field to a document using the specified attributes
/// </summary>
/// <param name="document">Document to add the form field to</param>
/// <param name="page_index">Page index</param>
/// <param name="rect">Form field area</param>
/// <param name="font">Font</param>
/// <param name="font_size">Font size</param>
/// <param name="field_name">Input field name</param>
/// <param name="result">(Output) result</param>
/// <returns>Combobox</returns>
FPDF_EXPORT IPDF_COMBOBOX __stdcall IPDF_AddFormSelectField(FPDF_DOCUMENT document, int page_index,
    FS_RECTF rect, IPDF_FONT font, unsigned int font_size, const wchar_t* field_name, IPDF_RESULT& resut);
/// <summary>
/// Add an option to a form combobox field
/// </summary>
/// <param name="object">Combo box to add the option to</param>
/// <param name="option"></param>
/// <returns>Result</returns>
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_AppendFormSelectFieldOption(FPDF_DOCUMENT document, IPDF_COMBOBOX object, const wchar_t* option);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_AddTextAnnotation(FPDF_DOCUMENT doc,const char* title,const char* subject,const char* content,int pageindex,float x,float y,float width,float height,int icontype,int flags,unsigned int red,unsigned int green,unsigned int blue,unsigned int alpha);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_ChangeImageObjectData(FPDF_DOCUMENT document, FPDF_PAGE page, FPDF_PAGEOBJECT obj, char* data, int size, int width, int height, int bpc, int num_comps, bool color_transform, const char* filter);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_ChangeImageMaskData(FPDF_DOCUMENT document, FPDF_PAGE page, FPDF_PAGEOBJECT obj, char* data, int size, int width, int height, int bpc, int num_comps, bool color_transform, const char* filter);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_CopyAcroForms(FPDF_DOCUMENT src_doc, FPDF_DOCUMENT dest_doc, int page_start_index, int page_count);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_CopyBookmarks(FPDF_DOCUMENT src_doc, FPDF_DOCUMENT dest_doc, const wchar_t* prev_toplevel, int page_offset);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_CopyMetadata(FPDF_DOCUMENT src_doc, FPDF_DOCUMENT dest_doc);

/// <summary>
/// Merges all the page objects from specified source document page append to destination document page
/// (source page will be foreground)
/// </summary>
/// <param name="src_doc">Document to copy objects from</param>
/// <param name="src_page_index">Page index of document to copy objects from</param>
/// <param name="dest_doc">Document to copy objects to</param>
/// <param name="dest_page_index">Page index of document to copy objects to</param>
/// <returns>Result</returns>
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_MergePages_Append(FPDF_DOCUMENT src_doc, int src_page_index, FPDF_DOCUMENT dest_doc, int dest_page_index);
/// <summary>
/// Merges all the page objects from specified source document page prepend to destination document page
/// (source page will be background)
/// </summary>
/// <param name="src_doc">Document to copy objects from</param>
/// <param name="src_page_index">Page index of document to copy objects from</param>
/// <param name="dest_doc">Document to copy objects to</param>
/// <param name="dest_page_index">Page index of document to copy objects to</param>
/// <returns>Result</returns>
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_MergePages_Prepend(FPDF_DOCUMENT src_doc, int src_page_index, FPDF_DOCUMENT dest_doc, int dest_page_index);
FPDF_EXPORT int __stdcall IPDF_GetBookmarkCount(FPDF_DOCUMENT document, IPDF_RESULT& result);
FPDF_EXPORT IPDF_RADIOBUTTONFIELD __stdcall IPDF_FindRadioButtonField(FPDF_DOCUMENT document, int page_index, const wchar_t* field_name, IPDF_RESULT& result);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_GetBookmark(FPDF_DOCUMENT document, int index, wchar_t*& text_out, int & text_out_len,
    wchar_t*& parent_text_out, int & parent_text_out_len, wchar_t*& previous_text_out, int & previous_text_out_len,
    wchar_t*& chain_text_out, int & chain_text_out_len, int & destination_pg_index_out);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_GetLastTopLevelBookmark(FPDF_DOCUMENT document, wchar_t*& text_out, int & text_out_len);
/// <summary>
/// Retrieve last error code
/// </summary>
/// <returns>Error code</returns>
FPDF_EXPORT IpdfErrorCodes __stdcall IPDF_GetErrorCode();
FPDF_EXPORT int __stdcall IPDF_GetFormFieldFlags(FPDF_DOCUMENT outline, const wchar_t* name, IPDF_RESULT& result);
FPDF_EXPORT int __stdcall IPDF_GetMetadataProperty(FPDF_DOCUMENT document, const char* name, wchar_t*& text_out, IPDF_RESULT& result);
FPDF_EXPORT int __stdcall IPDF_GetOwnerPassword(FPDF_DOCUMENT document, char*& text_out, IPDF_RESULT& result);
FPDF_EXPORT int __stdcall IPDF_GetUserPassword(FPDF_DOCUMENT document, char*& text_out, IPDF_RESULT& result);
FPDF_EXPORT int __stdcall IPDF_GetPermissions(FPDF_DOCUMENT document, IPDF_RESULT& result);
FPDF_EXPORT int __stdcall IPDF_GetSoftMask(FPDF_DOCUMENT document, FPDF_PAGE page, FPDF_PAGEOBJECT obj, char*& data_out, IPDF_RESULT& result, int& width, int& height);
FPDF_EXPORT int __stdcall IPDF_GetUniqueImageResources(FPDF_DOCUMENT document, IPDF_IMAGE*& obj_nums, IPDF_RESULT& result);
FPDF_EXPORT int __stdcall IPDF_GetImageObjectStreamNumber(FPDF_DOCUMENT document, FPDF_PAGEOBJECT obj, IPDF_RESULT& result);
/// <summary>
/// Initialize AcroForm for the specified document
/// </summary>
/// <param name="document">Document</param>
/// <param name="font">Default font</param>
/// <returns>Result</returns>
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_InitializeAcroForm(FPDF_DOCUMENT document, IPDF_FONT font);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_RenameFormField(FPDF_DOCUMENT document, const wchar_t* current_name, const wchar_t* new_name, wchar_t*& full_name_out, int & full_name_out_len);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_ShareImageData(FPDF_DOCUMENT document, FPDF_PAGEOBJECT src_obj, FPDF_PAGEOBJECT dst_obj);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_SetFormFieldFlagsByIndex(FPDF_DOCUMENT document, int page_index, int annotation_index, int flags);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_SetFormFieldFlags(FPDF_DOCUMENT outline, const wchar_t* name, int flags);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_SetMetadataProperty(FPDF_DOCUMENT document, const char* name, const wchar_t* value);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_SetPasswordAndPermissions(FPDF_DOCUMENT document, int permissions, const char* upassword, const char* opassword);
//FPDF_EXPORT IPDF_RESULT __stdcall IPDF_SetPasswordAndPermissions(FPDF_DOCUMENT document, int permissions, const char* upassword, const char* opassword);
    /// <summary>
/// Set print callback method
/// </summary>
/// <param name="callback">Callback method</param>
/// <returns>IPDF error code</returns>
FPDF_EXPORT void __stdcall IPDF_SetPrintCallback(Ipdf::DefaultEvent callback);
FPDF_PAGE drawrectangleorellipse(FPDF_DOCUMENT doc,FPDF_PAGE page,bool drawellipse,float top,float bottom,float left,float right,int borderred,int bordergreen,int borderblue,int alpha,int borderwidth,int borderstyle,int firstdashlength,int firstgaplength,int seconddashlength,int secondgaplength,float interiorred,int interiorgreen,int interiorblue,bool iscloudborder,int cloudintensity);
FPDF_PAGE drawline(FPDF_DOCUMENT doc,FPDF_PAGE page,const char* content,int x1,int y1,int x2,int y2,int linered,int linegreen,int lineblue,int alpha,int width,int linestyle,int firstdashlength,int firstgaplength,int seconddashlength,int secondgaplength,int beginingstyle,int endingstyle,float interiorred,float interiorgreen,float interiorblue,int leaderlinelength,int leaderlineextensionlength,int captionhorizontaloffset,int captionverticaloffset);
FPDF_PAGE drawpolygonorpolyline(FPDF_DOCUMENT doc,FPDF_PAGE page,bool drawpolygon,int points[],int red,int green,int blue,int alpha,int borderwidth,int linestyle,int firstdashlength,int firstgaplength,int seconddashlength,int secondgaplength,bool iscloudborder,int cloudintensity);
FPDF_PAGE drawtext(FPDF_DOCUMENT doc,FPDF_PAGE page,const char* content,int left,int right,int top,int bottom,int backgroundred,int backgroundgreen,int backgroundblue,int alpha,int fonttype,int textsize,const char* textcolor,int textaligntype,bool isbold,bool isitalic,bool isunderline,bool islinethrough,int borderwidth,int borderstyle,int firstdashlength,int firstgaplength,int seconddashlength,int secondgaplength,bool iscloudborder,int cloudintensity);
FPDF_PAGE addhyperlink(FPDF_DOCUMENT doc,FPDF_PAGE page,int top,int bottom,int left,int right,int gototype,const char* uri,const char* filepath,int destpageindex,int destpagefittype,int destleft,int destright,int desttop,int destbottom,int destzoom);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_InitPdfMetadata(FPDF_DOCUMENT document);
FPDF_EXPORT IPDF_RESULT __stdcall IPDF_CopyAnnotation(FPDF_DOCUMENT src_doc, int src_page_index, FPDF_DOCUMENT dest_doc, int dest_page_index);
#ifdef __cplusplus
}
#endif
#endif

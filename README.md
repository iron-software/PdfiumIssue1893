# PdfiumIssue1893
For https://bugs.chromium.org/p/pdfium/issues/detail?id=1893

## Description
Issue: Page content matrices are broken after removing an object

Input: elvis.pdf

Output: result.pdf

## Process
1. open document
2. load page & object
3. remove object
4. generate content
5. close page
6. save doc bytes to disk

## Before and After
![screenshot of before and after](https://github.com/iron-software/PdfiumIssue1893/blob/main/Before%20and%20After.png?raw=true)

## Which Object is Being Removed?
![screenshot of removed object](https://github.com/iron-software/PdfiumIssue1893/blob/main/Which%20Object%20is%20Removed.png?raw=true)

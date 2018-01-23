/*
 *
 *   Copyright (C) 2018 by C.H. Huang
 *   plushuang.tw@gmail.com
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  ---
 *
 *  In addition, as a special exception, the copyright holders give
 *  permission to link the code of portions of this program with the
 *  OpenSSL library under certain conditions as described in each
 *  individual source file, and distribute linked combinations
 *  including the two.
 *  You must obey the GNU Lesser General Public License in all respects
 *  for all of the code used other than OpenSSL.  If you modify
 *  file(s) with this exception, you may extend this exception to your
 *  version of the file(s), but you are not obligated to do so.  If you
 *  do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source
 *  files in the program, then also delete it here.
 *
 */

#ifndef UGET_FILES_H
#define UGET_FILES_H

#include <stdint.h>
#include <UgData.h>
#include <UgArray.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  UgetFiles           UgetFiles;
typedef struct  UgetFilesElement    UgetFilesElement;
typedef UG_ARRAY(UgetFilesElement)  UgetFilesArray;

extern const UgDataInfo*  UgetFilesInfo;

enum UgetFilesState
{
	UGET_FILES_FILE         = 0x0001,
	UGET_FILES_FOLDER       = 0x0002,
	UGET_FILES_ATTACHMENT   = 0x0004,

	// source
	UGET_FILES_IGNORE       = 0x0010,  // torrent and metalink

	UGET_FILES_DELETED      = 0x0400,  // this file was deleted
	UGET_FILES_COMPLETED    = 0x0800,

	UGET_FILES_CLEAR_MASK   = 0xF0FF,
};

struct UgetFilesElement
{
	char*  name;    // filename, It must be first member.
	int    state;   // UgetFilesState
};

// copy UgetFilesElement from src.
void uget_files_array_copy(UgetFilesArray* array, UgetFilesArray* src);

// ----------------------------------------------------------------------------
// UgetFiles: store all filename include source and downloaded filenames.

struct UgetFiles
{
	UG_DATA_MEMBERS;           // It derived from UgData
//	const UgDataInfo*  info;

	// files/folders from torrent/metalink (unsorted)
	UgetFilesArray  source;

	// files/folders actually write into storage device (sorted)
	UgetFilesArray  output;

	int  source_count;    // +1 if source changed
	int  output_count;    // +1 if output changed
};

int  uget_files_assign(UgetFiles* files, UgetFiles* src);


// sync UgetFiles::output and remove 'state = UGET_FILES_DELETED' element.
// keep_deleted: keep local.output 'state = UGET_FILES_DELETED' element.
//               remove src.output 'state = UGET_FILES_DELETED' element.
// return FALSE if local.output keep no change.
int  uget_files_sync(UgetFiles* local, UgetFiles* src, int keep_deleted);

UgetFilesElement*  uget_files_realloc(UgetFiles* files, const char* name);

// e.g. remove "foo.mp4" in UgFiles::output
// element = uget_files_realloc(src_files, "foo.mp4.aria2");
// element->state |= UGET_FILES_DELETED;
// uget_files_sync(files, src_files);

#ifdef __cplusplus
}
#endif

// ----------------------------------------------------------------------------
// C++11 standard-layout

#ifdef __cplusplus

namespace Uget
{

// These are for directly use only. You can NOT derived it.
struct Files : Ug::DataMethod, UgetFiles {};

};  // namespace Uget

#endif  // __cplusplus


#endif  // End of UGET_FILES_H

// ***************************************************************** -*- C++ -*-
/*
 * Copyright (C) 2004-2021 Exiv2 authors
 * This program is part of the Exiv2 distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301 USA.
 */
#ifndef ORFIMAGE_HPP_
#define ORFIMAGE_HPP_

// *****************************************************************************
#include "exiv2lib_export.h"

// included header files
#include "tiffimage.hpp"

// *****************************************************************************
// namespace extensions
namespace Exiv2 {

// *****************************************************************************
// class definitions

    // Add ORF to the supported image formats
    namespace ImageType {
        const int orf = 9;          //!< ORF image type (see class OrfImage)
    }

    /*!
      @brief Class to access raw Olympus ORF images. Exif metadata is supported
             directly, IPTC is read from the Exif data, if present.
     */
    class EXIV2API OrfImage : public TiffImage {
    public:
        //! @name NOT Implemented
        //@{
        //! Copy constructor
        OrfImage(const OrfImage& rhs) = delete;
        //! Assignment operator
        OrfImage& operator=(const OrfImage& rhs) = delete;
        //@}

        //! @name Creators
        //@{
        /*!
          @brief Constructor that can either open an existing ORF image or create
              a new image from scratch. If a new image is to be created, any
              existing data is overwritten. Since the constructor can not return
              a result, callers should check the good() method after object
              construction to determine success or failure.
          @param io An auto-pointer that owns a BasicIo instance used for
              reading and writing image metadata. \b Important: The constructor
              takes ownership of the passed in BasicIo instance through the
              auto-pointer. Callers should not continue to use the BasicIo
              instance after it is passed to this method.  Use the Image::io()
              method to get a temporary reference.
          @param create Specifies if an existing image should be read (false)
              or if a new file should be created (true).
         */
        OrfImage(BasicIo::UniquePtr io, bool create);
        //@}

        //! @name Manipulators
        //@{
        void printStructure(std::ostream& out, PrintStructureOption option, int depth) override;
        void readMetadata() override;
        void writeMetadata() override;
        /*!
          @brief Not supported. ORF format does not contain a comment.
              Calling this function will throw an Error(kerInvalidSettingForImage).
         */
        void setComment(const std::string& comment) override;
        //@}

        //! @name Accessors
        //@{
        std::string mimeType() const override;
        int pixelWidth() const override;
        int pixelHeight() const override;
        //@}
    }; // class OrfImage

    /*!
      @brief Stateless parser class for data in ORF format. Images use this
             class to decode and encode ORF data.
             See class TiffParser for details.
     */
    class EXIV2API OrfParser {
    public:
        /*!
          @brief Decode metadata from a buffer \em pData of length \em size
                 with data in ORF format to the provided metadata containers.
                 See TiffParser::decode().
        */
        static ByteOrder decode(
                  ExifData& exifData,
                  IptcData& iptcData,
                  XmpData&  xmpData,
            const byte*     pData,
                  uint32_t  size
        );
        /*!
          @brief Encode metadata from the provided metadata to ORF format.
                 See TiffParser::encode().
        */
        static WriteMethod encode(
                  BasicIo&  io,
            const byte*     pData,
                  uint32_t  size,
                  ByteOrder byteOrder,
            const ExifData& exifData,
            const IptcData& iptcData,
            const XmpData&  xmpData
        );
    }; // class OrfParser

// *****************************************************************************
// template, inline and free functions

    // These could be static private functions on Image subclasses but then
    // ImageFactory needs to be made a friend.
    /*!
      @brief Create a new OrfImage instance and return an auto-pointer to it.
             Caller owns the returned object and the auto-pointer ensures that
             it will be deleted.
     */
    EXIV2API Image::UniquePtr newOrfInstance(BasicIo::UniquePtr io, bool create);

    //! Check if the file iIo is an ORF image.
    EXIV2API bool isOrfType(BasicIo& iIo, bool advance);

}                                       // namespace Exiv2

#endif                                  // #ifndef ORFIMAGE_HPP_

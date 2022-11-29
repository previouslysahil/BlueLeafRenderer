//
//  ImageFactory.swift
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

import Foundation
import Cocoa

class ImageFactory {
    /// Uses the create method to generate a black image
    /// - Parameters:
    ///   - width: The width of our image
    ///   - height: The height of our image
    /// - Returns: CGImage generated from a buffer of black pixels
    func blackImage(width: Int, height: Int) -> CGImage {
        // Make a black image buffer (FF is 32 bit unsigned
        // reading 255 in base 10)
        var buffer = [UInt32](repeating: 0xFF, count: width * height)
        var cgImage: CGImage!
        // Make our cgimage using this buffer
        buffer.withUnsafeMutableBufferPointer { bufferPtr in
            cgImage = create(using: bufferPtr.baseAddress!, width: width, height: height)
        }
        return cgImage
    }
    /// Creates a CGImage from a buffer to an array of 32bit ints that represent a pixel's RGBA
    /// 8 bit values
    /// - Parameters:
    ///   - renderBuffer: The 32bit buffer of pixels
    ///   - width: The width of our image
    ///   - height: The height of our image
    /// - Returns: CGImage generated from this data
    func create(using renderBuffer: UnsafeMutablePointer<UInt32>, width: Int, height: Int) -> CGImage {
        // Make our colorspace and bitmap info
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        let bitmapInfo = CGImageAlphaInfo.premultipliedLast.rawValue | CGBitmapInfo.byteOrder32Little.rawValue
        // Create our cgImage
        var context: CGContext!
        context = CGContext(data: renderBuffer, width: width, height: height, bitsPerComponent: 8, bytesPerRow: width * 4, space: colorSpace, bitmapInfo: bitmapInfo, releaseCallback: nil, releaseInfo: nil)
        let cgImage = context.makeImage()
        return cgImage!
    }
}

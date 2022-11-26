//
//  ImageFactory.swift
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

import Foundation
import Cocoa

class ImageFactory {
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

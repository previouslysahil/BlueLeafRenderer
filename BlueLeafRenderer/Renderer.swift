//
//  Renderer.swift
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/25/22.
//

import Foundation
import Cocoa

/// This class is primarily just a wrapper of our c++ Renderer class handling
/// memory management of our c++ Renderer and renderBuffer alongside
/// producing our rendered image and publishing this image to our view
class Renderer: ObservableObject {
    
    @Published var image: CGImage?
    
    private var cRenderer: OpaquePointer!
    
    private let samples: Int
    private let width: Int
    private let height: Int
    private let factory = ImageFactory()
    
    init(width: Int, height: Int, samples: Int, bounces: Int) {
        cRenderer = Renderer_init(Int32(width), Int32(height), Int32(samples), Int32(bounces))
        self.samples = samples
        self.width = width
        self.height = height
    }
    
    /// Renderer our image for the given number of samples publishing
    /// the image to view after each sample is completed
    func createImage() {
        Task {
            print("Starting rendering")
            let start = CFAbsoluteTimeGetCurrent()
            for sample in 1...samples {
                print("Sample \(sample): Started rendering")
                let startRender = CFAbsoluteTimeGetCurrent()
                // We make our render using c++ renderer class
                let renderBuffer = Renderer_render_buffer(cRenderer, Int32(sample))
                let diffRender = CFAbsoluteTimeGetCurrent() - startRender
                print("Sample \(sample): Rendering took \(diffRender) seconds")
                // Only make and publish our image every 2 samples for performance
                if sample % 2 == 0 {
                    let startImageCreate = CFAbsoluteTimeGetCurrent()
                    // We create our cgImage using the 32bit pixel data of our render
                    let image = factory.create(using: renderBuffer!, width: width, height: height)
                    // Free our pointer to our buffer we are done with it
                    renderBuffer!.deallocate()
                    let diffImageCreate = CFAbsoluteTimeGetCurrent() - startImageCreate
                    print("Sample \(sample): Image create took \(diffImageCreate) seconds")
                    // Slow execution so we publish sparingly
                    await MainActor.run {
                        self.image = image
                    }
                }
            }
            let diff = CFAbsoluteTimeGetCurrent() - start
            print("Total render took \(diff) seconds")
        }
    }
    
    deinit {
        Renderer_deinit(cRenderer)
    }
}
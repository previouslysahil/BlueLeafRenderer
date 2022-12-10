//
//  Renderer.swift
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/25/22.
//

import Foundation
import SwiftUI
import Cocoa

/// This class is primarily just a wrapper of our c++ Renderer class handling
/// memory management of our c++ Renderer and renderBuffer alongside
/// producing our rendered image and publishing this image to our view
class Renderer: ObservableObject {
    
    @Published var image: CGImage
    @Published var sampleCount: Int
    @Published var state: RendererState
    
    private var cRenderer: OpaquePointer!
    
    private let samples: Int
    private let width: Int
    private let height: Int
    private let factory = ImageFactory()
    
    enum RendererState {
        case mint
        case running
        case done
    }
    
    init(width: Int, height: Int, samples: Int, bounces: Int) {
        // Create a blank black image to start
        self.image = factory.blackImage(width: width, height: height)
        self.sampleCount = 0
        self.state = .mint
        // Make our renderer
        cRenderer = Renderer_init(Int32(width), Int32(height), Int32(samples), Int32(bounces))
        self.samples = samples
        self.width = width
        self.height = height
    }
    
    /// Renderer our image for the given number of samples publishing
    /// the image to view after each sample is completed
    func createImage() {
        withAnimation(.spring()) {
            state = .running
        }
        Task {
            print("Starting rendering")
            let start = CFAbsoluteTimeGetCurrent()
            // Make an empty buffer we will use for rendering
            let renderBuffer = UnsafeMutableBufferPointer<UInt32>.allocate(capacity: width * height)
            for sample in 1...samples {
                print("Sample \(sample): Started rendering")
                let startRender = CFAbsoluteTimeGetCurrent()
                // We make our render using C++ renderer class (bridged by C) and store
                // the result in our renderBuffer
                Renderer_render_buffer(cRenderer, renderBuffer.baseAddress!, Int32(sample))
                let diffRender = CFAbsoluteTimeGetCurrent() - startRender
                print("Sample \(sample): Rendering took \(diffRender) seconds")
                // Create our image from our render buffer
                let startImageCreate = CFAbsoluteTimeGetCurrent()
                // We create our cgImage using the 32bit pixel data populated
                // into our renderBuffer
                let image = factory.create(using: renderBuffer.baseAddress!, width: width, height: height)
                let diffImageCreate = CFAbsoluteTimeGetCurrent() - startImageCreate
                print("Sample \(sample): Image create took \(diffImageCreate) seconds")
                // Slightly slows execution because this requires redrawing our view hiearchy
                await MainActor.run {
                    self.image = image
                    self.sampleCount = sample
                    // Set state to done if we are on last sample
                    if sample == samples {
                        withAnimation(.spring()) {
                            state = .done
                        }
                    }
                }
            }
            // Free our buffer pointer
            renderBuffer.deallocate()
            let diff = CFAbsoluteTimeGetCurrent() - start
            print("Total render took \(diff) seconds")
            // Somehting about maybe using a CAMetalLayer instead of SwiftUI.Image?
            // Printing to console adds to render times slightly
        }
    }
    
    /// Renderer our image for the given number of samples publishing
    /// the image to view after each tile sample is completed
    /// RUNS SLOWER THAN NON TILED APPROACH WHY?
    func createImageTiled(_ tile_size: Int) {
        withAnimation(.spring()) {
            state = .running
        }
        Task {
            print("Starting rendering")
            let start = CFAbsoluteTimeGetCurrent()
            // Make an empty buffer we will use for rendering
            let renderBuffer = UnsafeMutableBufferPointer<UInt32>.allocate(capacity: width * height)
            let h_rm = height % tile_size
            let w_rm = width % tile_size
            var j = height - 1
            var i = 0
            while j >= h_rm - 1 {
                while i <= width - w_rm {
                    for sample in 1...samples {
                        print("Sample \(sample): Started rendering")
                        let startRender = CFAbsoluteTimeGetCurrent()
                        // We make our render using C++ renderer class (bridged by C) and store
                        // the result in our renderBuffer
                        Renderer_render_buffer_tiled(cRenderer, renderBuffer.baseAddress!, Int32(sample), Int32(tile_size), Int32(j), Int32(i), Int32(h_rm), Int32(w_rm))
                        let diffRender = CFAbsoluteTimeGetCurrent() - startRender
                        print("Sample \(sample): Rendering took \(diffRender) seconds")
                        // Create our image from our render buffer
                        let startImageCreate = CFAbsoluteTimeGetCurrent()
                        // We create our cgImage using the 32bit pixel data populated
                        // into our renderBuffer
                        let image = factory.create(using: renderBuffer.baseAddress!, width: width, height: height)
                        let diffImageCreate = CFAbsoluteTimeGetCurrent() - startImageCreate
                        print("Sample \(sample): Image create took \(diffImageCreate) seconds")
                        // Slightly slows execution because this requires redrawing our view hiearchy
                        await MainActor.run {
                            self.image = image
                            self.sampleCount = sample
                        }
                    }
                    i += tile_size
                }
                j -= tile_size
                i = 0
            }
            await MainActor.run {
                withAnimation(.spring()) {
                    state = .done
                }
            }
            // Free our buffer pointer
            renderBuffer.deallocate()
            let diff = CFAbsoluteTimeGetCurrent() - start
            print("Total render took \(diff) seconds")
            // Somehting about maybe using a CAMetalLayer instead of SwiftUI.Image?
            // Printing to console adds to render times slightly
        }
    }
    
    deinit {
        // Make sure we clean up our rendering class since it's
        // init'd on the heap
        Renderer_deinit(cRenderer)
    }
}

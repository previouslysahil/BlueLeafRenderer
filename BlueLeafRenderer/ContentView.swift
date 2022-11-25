//
//  ContentView.swift
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

import SwiftUI

struct ContentView: View {
    
    @State var tapped = false
    @State var image: CGImage?
    
    private let factory = ImageFactory()
    private let width = 400
    private let height = 225
    
    var body: some View {
        ZStack {
            Color(red: 1, green: 1, blue: 1)
            VStack {
                ForEach(0..<35) { _ in
                    HStack {
                        ForEach(0..<35) { _ in
                            Circle()
                                .fill(Color(red: 0.894, green: 0.912, blue: 0.958))
                                .frame(width: 3, height: 3)
                        }
                    }
                }
            }
            .padding(.top, 32)
            .padding(.bottom, 4)
            .padding(.horizontal)
            VStack {
                Spacer()
                    .frame(height: 32)
                if let image = image {
                    Image(image, scale: 1, label: Text("image"))
                        .resizable()
                        .frame(width: CGFloat(width), height: CGFloat(height))
                } else {
                    Spacer()
                        .frame(width: CGFloat(width), height: CGFloat(height))
                }
                Text("Rendered image")
                    .font(.custom("Avenir Next", size: 16))
                    .fontWeight(.light)
                    .foregroundColor(Color(red: 0, green: 0, blue: 0))
            }
            .padding()
        }
        .edgesIgnoringSafeArea(.all)
        .task {
            // Still seems to be running on main thread
            print("Started rendering")
            let startRender = CFAbsoluteTimeGetCurrent()
            let renderer = Renderer_init(Int32(width), Int32(height), Int32(100))
            let renderBuffer = Renderer_render_buffer(renderer)
            let diffRender = CFAbsoluteTimeGetCurrent() - startRender
            print("Rendering took \(diffRender) seconds")
            let startImageCreate = CFAbsoluteTimeGetCurrent()
            let image = factory.create(using: renderBuffer!, width: width, height: height)
            let diffImageCreate = CFAbsoluteTimeGetCurrent() - startImageCreate
            print("Image create took \(diffImageCreate) seconds")
            await MainActor.run {
                withAnimation(.spring()) {
                    self.image = image
                }
            }
            // Deinitalize our renderer when we are done with it
            Renderer_deinit(renderer)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

//
//  ContentView.swift
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

import SwiftUI

fileprivate let width = 400
fileprivate let height = 225

struct ContentView: View {
    
    @StateObject var renderer = Renderer(width: width, height: height, samples: 100, bounces: 50)
    
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
                if let image = renderer.image {
                    Image(image, scale: 1, label: Text("image"))
                        .resizable()
                        .frame(width: CGFloat(width), height: CGFloat(height))
                        .clipShape(RoundedRectangle(cornerRadius: 20))
                } else {
                    RoundedRectangle(cornerRadius: 20)
                        .fill(Color(red: 0, green: 0, blue: 0))
                        .frame(width: CGFloat(width), height: CGFloat(height))
                }
                // Updating text every sample nearly doubles
                // render time
                Text("Sample: \(renderer.sampleCount)")
                    .font(.custom("Avenir Next", size: 16))
                    .fontWeight(.light)
                    .foregroundColor(Color(red: 0, green: 0, blue: 0))
            }
            .padding()
        }
        .edgesIgnoringSafeArea(.all)
        .onAppear {
            renderer.createImage()
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

//
//  RendererView.swift
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/28/22.
//

import SwiftUI

fileprivate let width = 400
fileprivate let height = 225

struct RendererView: View {
    
    @StateObject var renderer = Renderer(width: width, height: height, samples: 1000, bounces: 50)
    
    var body: some View {
        VStack {
            Spacer()
                .frame(height: 32)
            ZStack {
                Image(renderer.image, scale: 1, label: Text("image"))
                    .resizable()
                    .frame(width: CGFloat(width), height: CGFloat(height))
                    .clipShape(RoundedRectangle(cornerRadius: 20))
                if renderer.state == .mint {
                    Text("Tap to render")
                        .font(.custom("Avenir Next", size: 12))
                        .fontWeight(.light)
                        .foregroundColor(Color(red: 1, green: 1, blue: 1))
                }
            }
            .onTapGesture {
                // Only allow image creation if we are not
                // already rendering
                if renderer.state != .running {
                    renderer.createImage()
                }
            }
            // Updating text every sample nearly doubles
            // render time
            if renderer.state == .running {
                Text("\(renderer.sampleCount) samples")
                    .font(.custom("Avenir Next", size: 16))
                    .fontWeight(.medium)
                    .foregroundColor(Color(red: 0, green: 0, blue: 0))
                    .transition(.opacity)
            }
        }
        .padding()
    }
}

struct RendererView_Previews: PreviewProvider {
    static var previews: some View {
        RendererView()
    }
}

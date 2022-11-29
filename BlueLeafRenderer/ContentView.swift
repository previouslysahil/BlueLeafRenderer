//
//  ContentView.swift
//  BlueLeafRenderer
//
//  Created by Sahil Srivastava on 11/23/22.
//

import SwiftUI

struct ContentView: View {
    
    var body: some View {
        ZStack {
            Color(red: 1, green: 1, blue: 1)
            VStack {
                ForEach(0..<35) { _ in
                    HStack {
                        ForEach(0..<35) { _ in
                            Circle()
                                .fill(Color(red: .random(in: 0...1), green: .random(in: 0...1), blue: .random(in: 0...1), opacity: .random(in: 0...0.4)))
                                .frame(width: 3, height: 3)
                        }
                    }
                }
            }
            .padding(.top, 32)
            .padding(.bottom, 4)
            .padding(.horizontal)
            .drawingGroup() // slightly helps in performance
            // Abstract renderer view so SwiftUI only redraws this view hierarchy when updating sample count and image
            RendererView()
        }
        .edgesIgnoringSafeArea(.all)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

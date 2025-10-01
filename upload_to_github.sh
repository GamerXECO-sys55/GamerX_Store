#!/bin/bash

# GamerX Store GitHub Upload Script
# Run this script to upload your repository to GitHub

echo "🚀 GamerX Store GitHub Upload Script"
echo "====================================="

# Check if we're in the right directory
if [ ! -f "meson.build" ] || [ ! -d "src" ]; then
    echo "❌ Error: Please run this script from the gamerx-store directory"
    exit 1
fi

echo "📁 Current directory: $(pwd)"
echo "📋 Preparing repository for upload..."

# Initialize git repository
echo "🔧 Initializing Git repository..."
git init

# Add all files
echo "📦 Adding files to Git..."
git add .

# Create initial commit
echo "💾 Creating initial commit..."
git commit -m "Initial commit: GamerX Store - Complete rebrand from Bazaar

- Renamed all files from bz-* to gx-* prefix
- Updated all function names, types, and macros
- Changed application ID to io.github.gamerx.GamerXStore
- Updated repository URLs to GamerXECO-sys55/GamerX_Store
- Renamed bazaar folder to gamerx-store
- Updated UI component from bazaar-ui to gamerx-store-ui
- Complete structural rename and cleanup
- Ready for production use"

# Add remote repository
echo "🌐 Adding GitHub remote..."
echo "Please enter your GitHub repository URL:"
echo "Example: https://github.com/GamerXECO-sys55/GamerX_Store.git"
read -p "Repository URL: " REPO_URL

git remote add origin "$REPO_URL"

# Set main branch
git branch -M main

echo ""
echo "✅ Repository prepared successfully!"
echo ""
echo "🔑 NEXT STEPS (you need to do these manually):"
echo "1. Run: git push -u origin main"
echo "2. Enter your GitHub username when prompted"
echo "3. Enter your GitHub personal access token (NOT password)"
echo ""
echo "📖 How to get a Personal Access Token:"
echo "1. Go to GitHub.com → Settings → Developer settings → Personal access tokens"
echo "2. Generate new token (classic)"
echo "3. Select 'repo' permissions"
echo "4. Copy the token and use it instead of password"
echo ""
echo "🎉 Your GamerX Store is ready for upload!"

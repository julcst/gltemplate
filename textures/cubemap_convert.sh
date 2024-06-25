#!/bin/bash

echo "Usage: $0 input_image size output_folder"

input_image="$1"
size="$2"
width=$size
height=$size
output_folder="${input_image%.*}"

# Create output directory if it doesn't exist
mkdir -p "$output_folder"

# Define the mappings of positions to cubemap faces along with flip flags
# Format: "row_col face flip_flag"
# flip_flag: "none" (no flip), "rotate" (rotate 180 degrees), "fliph" (flip horizontally), "flipv" (flip vertically)
positions=(
  "0_1 py none"
  "1_0 nx none"
  "1_1 pz none"
  "1_2 px none"
  "2_1 ny none"
  "3_1 nz rotate"
)

# Loop through the positions and extract the corresponding sub-images
for position in "${positions[@]}"; do
  row_col="${position% * *}"
  face_flip="${position#* }"
  face="${face_flip% *}"
  flip_flag="${face_flip#* }"
  
  row="${row_col%_*}"
  col="${row_col#*_}"
  
  x_offset=$((col * width))
  y_offset=$((row * height))
  
  output_image="${output_folder}/${face}.hdr"
  echo "${width}x${height}+$x_offset+$y_offset $output_image"
  
  case "$flip_flag" in
    "rotate")
      magick "$input_image" -crop ${width}x${height}+$x_offset+$y_offset -rotate 180 "$output_image"
      ;;
    "none")
      magick "$input_image" -crop ${width}x${height}+$x_offset+$y_offset "$output_image"
      ;;
    "fliph")
      magick "$input_image" -crop ${width}x${height}+$x_offset+$y_offset -flop "$output_image"
      ;;
    "flipv")
      magick "$input_image" -crop ${width}x${height}+$x_offset+$y_offset -flip "$output_image"
      ;;
    *)
      echo "Unknown flip flag: $flip_flag"
      ;;
  esac
done

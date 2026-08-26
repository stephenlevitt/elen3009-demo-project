This directory contains images that are referenced from the project
documentation (`architecture.md`). Do not place game assets here - those belong
in the `resources` folder. This folder is for documentation images only.

## Adding an image

1. Copy your image file into this folder.
2. Reference it from `architecture.md` using Markdown:

   ```markdown
   ![Image description](architecture-images/your-image.png)
   ```

The image is automatically included in the generated HTML documentation
(`doxygen/html/`).

## Supported image types

The following image formats are supported:

| Format | Extension(s) |
| ------ | ------------ |
| PNG    | `.png`       |
| JPEG   | `.jpg`, `.jpeg` |
| SVG    | `.svg`       |

**Recommendation:** use SVG/PNG for diagrams.

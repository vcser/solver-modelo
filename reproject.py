import sys
import rasterio
from rasterio.warp import calculate_default_transform, reproject, Resampling

def reproject_to_epsg4326(src_path, dst_path):
    dst_crs = "EPSG:4326"

    with rasterio.open(src_path) as src:
        transform, width, height = calculate_default_transform(
            src.crs, dst_crs, src.width, src.height, *src.bounds)
        
        kwargs = src.meta.copy()
        kwargs.update({
            "crs": dst_crs,
            "transform": transform,
            "width": width,
            "height": height
        })

        with rasterio.open(dst_path, 'w', **kwargs) as dst:
            for i in range(1, src.count + 1):
                reproject(
                    source=rasterio.band(src, i),
                    destination=rasterio.band(dst, i),
                    src_transform=src.transform,
                    src_crs=src.crs,
                    dst_transform=transform,
                    dst_crs=dst_crs,
                    resampling=Resampling.nearest
                )

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python reproject_raster.py entrada.tif salida_4326.tif")
        sys.exit(1)

    src_file = sys.argv[1]
    dst_file = sys.argv[2]

    reproject_to_epsg4326(src_file, dst_file)
    print(f"Archivo reproyectado guardado como: {dst_file}")
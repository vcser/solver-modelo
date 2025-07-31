import sys
import rasterio
from rasterio.transform import rowcol
import json

def get_value_from_tif(tif_path, lon, lat):
    with rasterio.open(tif_path) as src:
        print("archivo: ", tif_path)
        print("crs: ", src.crs)
        print("bounds: ", src.bounds)
        print("anchura x altura: ", src.width, "x", src.height)
        print("transform: ", src.transform)
        row, col = rowcol(src.transform, lon, lat)
        value = src.read(1)[row, col]
        return value

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python query_tif.py <lat> <lon> <tif pendiente> <tif modelo combustible>", file=sys.stderr)
        sys.exit(1)

    lat = float(sys.argv[1])
    lon = float(sys.argv[2])
    tifPendiente = sys.argv[3]
    tifModeloCombustible = sys.argv[4]

    try:
        pendiente = get_value_from_tif(tifPendiente, lon, lat)
        modeloCombustible = get_value_from_tif(tifModeloCombustible, lon, lat)
        result = {
            "pendiente": float(pendiente),
            "modeloCombustible": float(modeloCombustible)
        }
        print(json.dumps(result))
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(2)

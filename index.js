let map;

async function initMap() {
  const { Map, InfoWindow } = await google.maps.importLibrary("maps");
  const { AdvancedMarkerElement, PinElement } = await google.maps.importLibrary("marker")

  const infoWindow = new InfoWindow();

  const map = new google.maps.Map(document.getElementById('map'), {
    center: { lat: 38.0336, lng: -78.5080 },
    zoom: 16,
    mapId: 'abcde',
    disableDefaultUI: true,
  });

  const greenBin = document.createElement("img");
  greenBin.src =
    "./images/trash_can_green.png";
  greenBin.style.width = "50px"; // Set the width to 100 pixels
  greenBin.style.height = "60px";

  const greenBinPin = new PinElement({
    scale: 0.5,
  });

  const marker = new google.maps.marker.AdvancedMarkerElement({
    map,
    position: { lat: 38.0336, lng: -78.5080 },
    title: "trashcan1",
    content: greenBin,
  });

  marker.addListener("click", () => {
    console.log("clicked");
    infoWindow.close()
    infoWindow.setContent(marker.title);
    infoWindow.open(marker.map, marker);
  });
}

initMap();
let map;

async function initMap(data) {
  console.log(data)
  const { Map, InfoWindow } = await google.maps.importLibrary("maps")
  const { AdvancedMarkerElement, PinElement } = await google.maps.importLibrary("marker")

  const infoWindow = new InfoWindow();

  const map = new google.maps.Map(document.getElementById("map"), {
    center: { lat: 38.0336, lng: -78.5080 },
    zoom: 16,
    mapId: "abcde",
  });

  const greenBin = document.createElement("img")
  greenBin.src =
    "./images/trash_can_green.png"
  greenBin.style.width = "50px"
  greenBin.style.height = "60px"

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
    console.log("clicked")
    infoWindow.close()
    infoWindow.setContent(marker.title)
    infoWindow.open(marker.map, marker)
  });
}

async function fetchData(url) {
  try {
    const response = await fetch(url)
    if (!response.ok) {
      throw new Error("Network response was not ok")
    }
    const data = await response.json()
    return data
  } catch (error) {
    console.error("Error fetching data:", error)
    return null
  }
}

async function initMapWithData() {
  const data = await fetchData(backendUrl)
  if (data) {
    initMap(data)
  } else {
    console.error('Failed to fetch data.')
  }
}

// initMapWithData();

initMap("hi")
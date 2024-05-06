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

  const bin = document.createElement("img")
  if(data.value.Distance>30){
    bin.src="./images/trash_can_green.png"
  }
  else if(data.value.Distance>10){
    bin.src="./images/trash_can_yellow.png"
  }
  else{
    bin.src="./images/trash_can_red.png"
  }
  bin.style.width = "50px"
  bin.style.height = "55px"

  const marker = new google.maps.marker.AdvancedMarkerElement({
    map,
    position: { lat: 38.03343, lng: -78.50985 },
    title: (""+data.value.ID),
    content: bin,
  });

  const distanceToBottomOfBin = 40

  let tempDistance = data.value.Distance
  if(tempDistance>37.5){
    tempDistance = 40
  }
  let distancePercent = Number(((distanceToBottomOfBin-tempDistance)/distanceToBottomOfBin*100).toFixed(1))

  let html = (
    "<h2>Bin ID: " + marker.title + "</h2>" + 
    "<h3>Distance: " + Number(data.value.Distance.toFixed(3)) + " cm</h3>" +
    "<h3>" + distancePercent + "% full</h3>"
  )

  marker.addListener("click", () => {
    console.log("clicked")
    infoWindow.close()
    infoWindow.setContent(html)
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

initMapWithData();
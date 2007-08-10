function search(element) {
  // Get a list of divs in the document
  var divs = element.getElementsByTagName("div");
  for (var i = 0; divs[i] != null; i++) {
    // If the divs are "boxes", boxify them (mimick :before/:after)
    if (divs[i].className.match(/^box_[a-z]+$/)) {
      boxify(divs[i], divs[i].className);
    // If the div is the "download bubble," do the same.
    } else if (divs[i].id == "download") {
      boxify(divs[i], divs[i].id);
    }
  }
}

// Create a box before and after the element containing its borders
function boxify(element, type) {
  before = document.createElement("div");
  before.className = type + "_before";
  before.innerHTML = "&nbsp;";
  element.insertBefore(before, element.firstChild);

  after = document.createElement("div");
  after.className = type + "_after";
  after.innerHTML = "&nbsp;";
  element.insertBefore(after);
}

search(document);

(function() {
  function findModule(name) {
    return commandReference.modules.find(function(m) {
      return m.name === name
    })
  }

  function findCommand(name) {
    return commandReference.commands.find(function(c) {
      return c.name === name
    })
  }

  var params = new URLSearchParams(window.location.search.slice(1))

  var activeCommand = findCommand(params.get("command"))
  var activeModule  = findModule(params.get("module"))

  function createNavbar() {
    var moduleList = document.querySelector(".command-list > ul")

    commandReference.modules.forEach(function(mod) {
      var moduleLink = document.createElement("a")
      moduleLink.setAttribute("href", "?module=" + mod.name)
      moduleLink.innerText = mod.name

      var commandList = document.createElement("ul")
      mod.commands.forEach(function(commandName) {
        var command = findCommand(commandName)

        var commandLink = document.createElement("a")
        commandLink.setAttribute("href", "?command=" + command.name)
        commandLink.innerText = command.name

        if (activeCommand && activeCommand == command) {
          commandLink.classList.add("active")
        }

        var commandItem = document.createElement("li")
        commandItem.classList.add("command")
        commandItem.appendChild(commandLink)

        commandList.appendChild(commandItem)
      })


      var moduleItem = document.createElement("li")
      moduleItem.classList.add("module")
      moduleItem.appendChild(moduleLink)
      moduleItem.appendChild(commandList)

      moduleList.appendChild(moduleItem)
    })
  }

  // TODO: this could easily be sped up...
  search.addEventListener("keyup", function() {
    var query = search.value.toLowerCase()

    if (query.replace(/s+/g).length == 0){
      document.querySelectorAll('.command').forEach(function(n) {
        n.classList.remove("hidden")
      })
      document.querySelectorAll('.module').forEach(function(n) {
        n.classList.remove("hidden")
      })
    } else {
      document.querySelectorAll('.command').forEach(function(n) {
        var identifier = n.querySelector('a').innerText;
        identifier.toLowerCase().indexOf(query) > -1 ? n.classList.remove("hidden"): n.classList.add("hidden")
      })

      document.querySelectorAll('.module').forEach(function(n) {
        n.querySelectorAll('.command:not(.hidden)').length > 0 ? n.classList.remove("hidden"): n.classList.add("hidden")
      })
    }
  })

  createNavbar()

  var main = document.querySelector(".main")
  if (activeCommand) {
    main.innerHTML = activeCommand.content
  } else if (activeModule) {
    main.innerHTML = "<h1>" + activeModule.name + "</h1><p>" + activeModule.description + "</p>"
  }
})()

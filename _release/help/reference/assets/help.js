jQuery(function() {
  $('#search').on('keyup', function(e) {
    var query = $(this).val().toLowerCase()
    if (query.replace(/s+/g).length == 0){
      $('.command').show()
      $('.module').show()
    } else {
      $('.command').each(function() {
        var identifier = $('a', this).text()
        identifier.toLowerCase().indexOf(query) > -1 ? $(this).show() : $(this).hide()
      })

      $('.module').each(function() {
        $('.command:visible', this).length > 0 ? $(this).show() : $(this).hide()
      })
    }
  })
})

class String
  def indent
    split("\n").join("\n  ")
  end
end

require_relative './decl'

require_relative './node/base'
require_relative './node/invalid'

require_relative './node/arith_expr'
require_relative './node/ass'
require_relative './node/call'
require_relative './node/case'
require_relative './node/cast'
require_relative './node/decl_stmt'
require_relative './node/decl_var'
require_relative './node/delete_each'
require_relative './node/expr_stmt'
require_relative './node/field_var'
require_relative './node/float_const'
require_relative './node/func_decl'
require_relative './node/ident_var'
require_relative './node/if'
require_relative './node/int_const'
require_relative './node/for'
require_relative './node/for_each'
require_relative './node/new'
require_relative './node/prog'
require_relative './node/rel_expr'
require_relative './node/repeat'
require_relative './node/return'
require_relative './node/select'
require_relative './node/stmt_seq'
require_relative './node/string_const'
require_relative './node/var_decl'
require_relative './node/var_expr'
require_relative './node/while'

require_relative './type/base'
require_relative './type/invalid'
require_relative './type/int'
require_relative './type/float'
require_relative './type/func'
require_relative './type/string'
require_relative './type/struct'

require 'minitest/autorun'
require 'net/http'

class TestServer < MiniTest::Unit::TestCase
  def setup
    
  end

  def test_that_kitty_can_eat
    assert_equal "OHAI!", @meme.i_can_has_cheezburger?
  end

  def test_server_responds
    assert 1==1
  end

end
